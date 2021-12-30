#include "ManagementSession.hpp"
#include <boost/json.hpp>
#include <iostream>

namespace Management {

Session::Session(boost::asio::ip::tcp::socket&& socket, std::shared_ptr<std::string const> const& doc_root)
    : stream{std::move(socket)}, sendLambda{*this} {}

void Session::Run() {
    boost::asio::dispatch(this->stream.get_executor(), boost::beast::bind_front_handler(&Session::DoRead, shared_from_this()));
}

void Session::DoRead() {
    // Make the request empty before reading, otherwise the operation behavior is undefined.
    this->request = {};
    this->stream.expires_after(std::chrono::seconds(30));
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
        std::cout << "Target: " << this->request.target().data() << std::endl;
        if (this->request.method() == boost::beast::http::verb::get) {
//            std::cout << "Got get" << std::endl;
//
//            boost::beast::http::response<boost::beast::http::string_body> res{boost::beast::http::status::ok, request.version()};
//
//            std::cout << "1" << std::endl;
//            boost::json::object jsonObject{};
//
//            jsonObject["abcd"] = "66";
//            jsonObject["add"] = "77";
//
//            std::string value = boost::json::serialize(jsonObject);
//            res.body() = value;
//            std::cout << "3" << std::endl;
//
//            this->sendLambda(std::move(res));
//            std::cout << "4" << std::endl;

            if(request.target()[0] == '/') {
                Statistics::Controller<SendLambda> statisticsController{this->sendLambda};

                statisticsController.Process(this->request);
            } else {
                std::cout << "Invalid request target path" << std::endl;
            }
        } else {
            std::cout << "Got something else" << std::endl;
        }
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
