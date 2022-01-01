#pragma once
#include "../Source/DhcpPacket/DhcpPacket.hpp"
#include "../Source/Statistics/DhcpStatistics.hpp"
#include "DhcpAddressesCache.hpp"
#include "DhcpRequestsHandlers.hpp"
#include "Message.hpp"
#include <boost/asio.hpp>

namespace Dhcp {

constexpr size_t DefaultServerPort = 67;
constexpr size_t DefaultMaxMessageSize = 1024;

class Server {
private:
    std::shared_ptr<Dhcp::Statistics> dhcpStatistics;
    AddressesCache& dhcpAddressesCache;
    boost::asio::ip::udp::socket socket;
    boost::asio::ip::udp::endpoint remoteEndpoint;
    boost::asio::ip::udp::socket writeSocket;
    Message<Packet> message;
    std::list<std::unique_ptr<RequestHandler>> requestsHandlers;

    void HandleReceive(const boost::system::error_code&, std::size_t);
    void ProcessRequest(Packet&);
    void ProcessResponse(Packet&);

public:
    Server(boost::asio::io_context& ioContext, std::shared_ptr<Dhcp::Statistics>, AddressesCache&);
    ~Server();
    Server(const Server&) = delete;
    Server& operator=(const Server& other) = delete;
    Server(Server&&) = delete;
    Server& operator=(const Server&& other) = delete;

    void Listen();
};

} // namespace Dhcp
