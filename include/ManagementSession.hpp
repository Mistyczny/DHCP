#pragma once
#include "Configuration.hpp"
#include "ConfigurationController.hpp"
#include "DhcpStatistics.hpp"
#include "StatisticsController.hpp"
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <memory>

namespace Management {

class Session : public std::enable_shared_from_this<Session> {
private:
    struct SendLambda {
        Session& self;

        explicit SendLambda(Session& self) : self(self) {}

        template <bool isRequest, class Body, class Fields>
        void operator()(boost::beast::http::message<isRequest, Body, Fields>&& msg) const {
            // The lifetime of the message has to extend for the duration of the async operation so we use a shared_ptr to manage it.
            auto sp = std::make_shared<boost::beast::http::message<isRequest, Body, Fields>>(std::move(msg));

            // Store a type-erased version of the shared pointer in the class to keep it alive.
            this->self.response = sp;

            // Write the response
            boost::beast::http::async_write(
                this->self.stream, *sp,
                boost::beast::bind_front_handler(&Session::OnWriteCompleted, this->self.shared_from_this(), sp->need_eof()));
        }
    };

    boost::beast::tcp_stream stream;
    std::shared_ptr<Dhcp::Statistics> statistics;
    std::shared_ptr<Configuration::Config> configuration;
    boost::beast::flat_buffer buffer;
    std::shared_ptr<std::string const> doc_root;
    boost::beast::http::request<boost::beast::http::string_body> request;
    std::shared_ptr<void> response;
    std::map<std::string, std::unique_ptr<HttpControllerInterface<SendLambda>>> controllers;
    SendLambda sendLambda;

    void DoRead();
    void OnReadCompleted(boost::beast::error_code, std::size_t);
    void OnWriteCompleted(bool close, boost::beast::error_code ec, std::size_t bytes_transferred);

public:
    Session(boost::asio::ip::tcp::socket&& socket, std::shared_ptr<std::string const> const& doc_root,
            std::shared_ptr<Configuration::Config> config, std::shared_ptr<Dhcp::Statistics> statistics);

    void Run();
    void Close();
};

} // namespace Management
