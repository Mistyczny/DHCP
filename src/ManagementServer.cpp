#include "ManagementServer.hpp"
#include <iostream>

namespace Management {

class Listener : public std::enable_shared_from_this<Listener> {
private:
    boost::asio::io_context& ioContext;
    boost::asio::ip::tcp::acceptor acceptor;
    std::shared_ptr<std::string const> doc_root_;

    void OnAccept(boost::beast::error_code errorCode, boost::asio::ip::tcp::socket socket) {
        if (errorCode) {
            return;
        } else {
            std::make_shared<Session>(std::move(socket), doc_root_)->Run();
        }

        this->StartAcceptingConnections();
    }

public:
    explicit Listener(boost::asio::io_context& ioContext) : ioContext{ioContext}, acceptor(boost::asio::make_strand(ioContext)) {
        boost::beast::error_code errorCode{};
        if (this->acceptor.open(boost::asio::ip::tcp::v4(), errorCode); errorCode) {
            throw std::runtime_error("Failed to open management port");
        } else if (this->acceptor.set_option(boost::asio::socket_base::reuse_address(true), errorCode); errorCode) {
            throw std::runtime_error("Failed to set reuse address option");
        } else if (this->acceptor.bind(boost::asio::ip::tcp::endpoint(boost::asio::ip::make_address("127.0.0.1"), 12345), errorCode);
                   errorCode) {
            throw std::runtime_error("Failed to bind to port 12345");
        } else if (this->acceptor.listen(boost::asio::socket_base::max_listen_connections, errorCode); errorCode) {
            throw std::runtime_error("Failed to set reuse address option");
        } else {
            std::cout << "Management Server Listener created" << std::endl;
        }
    }

    void StartAcceptingConnections() {
        // The new connection gets its own strand
        this->acceptor.async_accept(boost::asio::make_strand(this->ioContext),
                                    boost::beast::bind_front_handler(&Listener::OnAccept, shared_from_this()));
    }
};

Server::Server(boost::asio::io_context& ioContext) : ioContext{ioContext} { listener = std::make_shared<Listener>(this->ioContext); }

void Server::Run() {
    if (this->listener) {
        this->listener->StartAcceptingConnections();
    }
}

} // namespace Management