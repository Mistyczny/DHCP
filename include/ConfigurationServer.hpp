#pragma once
#include <boost/asio.hpp>

namespace Configuration {

constexpr size_t DefaultServerPort = 8888;
constexpr size_t DefaultMaxMessageSize = 1024;

class Server {
private:
    boost::asio::io_context& ioContext;
    boost::asio::ip::udp::socket socket;
    boost::asio::ip::udp::endpoint remoteEndpoint;
    std::array<char, DefaultMaxMessageSize> buffer;

public:
    explicit Server(boost::asio::io_context& ioContext);
    ~Server() = default;

    void Listen();
};

} // namespace Configuration