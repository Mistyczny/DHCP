#include "ManagementSession.hpp"
#include "ConfigurationController.hpp"
#include "HttpControllerInterface.hpp"
#include <boost/json.hpp>
#include <iostream>

namespace Management {

Session::Session(boost::asio::ip::tcp::socket&& socket, std::shared_ptr<std::string const> const& doc_root,
                 std::shared_ptr<Dhcp::Statistics> dhcpStatistics)
    : stream{std::move(socket)}, dhcpStatistics{std::move(dhcpStatistics)}, sendLambda{*this} {

    controllers.emplace("/configuration", std::make_unique<Configuration::Controller<SendLambda>>(this->sendLambda));
    controllers.emplace("/statistics", std::make_unique<Statistics::Controller<SendLambda>>(this->sendLambda, this->dhcpStatistics));
}

void Session::Run() {
    boost::asio::dispatch(this->stream.get_executor(), boost::beast::bind_front_handler(&Session::DoRead, shared_from_this()));
}

void Session::DoRead() {
    // Make the request empty before reading, otherwise the operation behavior is undefined.
    this->request = {};
    this->stream.expires_after(std::chrono::seconds(60));
    boost::beast::http::async_read(this->stream, this->buffer, this->request,
                                   boost::beast::bind_front_handler(&Session::OnReadCompleted, shared_from_this()));
}

void Session::OnReadCompleted(boost::beast::error_code errorCode, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);
    std::cout << "READ!" << std::endl;
    if (errorCode == boost::beast::http::error::end_of_stream) {
        return this->Close();
    } else if (errorCode) {
        std::cout << "Error: " << errorCode.message() << std::endl;
    } else {
        std::cout << "Handle request there" << std::endl;
        std::cout << "Target: " << this->request.target().to_string() << std::endl;

        // We do not to care, if string has any length. In case it's empty find will return string::npos
        auto firstPos = this->request.target().find('/');
        auto secondPos = this->request.target().find('/', firstPos + 1);
        std::cout << firstPos << std::endl;
        std::cout << secondPos << std::endl;
        if(firstPos != std::string::npos && secondPos != std::string::npos) {
            auto target = this->request.target().substr(firstPos, secondPos).to_string();
            std::cout << "We have first target: " << target << std::endl;

            auto controller = this->controllers.find(target);
            if(controller != this->controllers.end()) {
                controller->second->Process(request);
            }
        } else {
            std::cout << "Invalid path" << std::endl;
        }

        this->DoRead();
    }
}

void Session::Close() {
    boost::beast::error_code errorCode;
    this->stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_send, errorCode);
    if (errorCode) {
        std::cout << "Closed with error: " << errorCode.message() << std::endl;
    }
}

void Session::OnWriteCompleted(bool close, boost::beast::error_code ec, std::size_t bytes_transferred) {}

} // namespace Management
