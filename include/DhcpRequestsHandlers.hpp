#pragma once
#include "DhcpPacket.hpp"
#include "DhcpStatistics.hpp"
#include <boost/asio.hpp>

namespace Dhcp {

class RequestHandler {
protected:
    boost::asio::ip::udp::socket& socket;
    std::shared_ptr<Dhcp::Statistics> dhcpStatistics;

    void sendResponse(PDU&&);

public:
    RequestHandler(boost::asio::ip::udp::socket& socket, std::shared_ptr<Dhcp::Statistics> dhcpStatistics);
    ~RequestHandler() = default;

    virtual bool Process(const Packet& packet) = 0;
};

class DiscoverRequestHandler : public RequestHandler {
private:
public:
    DiscoverRequestHandler(boost::asio::ip::udp::socket& socket, std::shared_ptr<Dhcp::Statistics> dhcpStatistics);

    bool Process(const Packet& packet) override;
};

class AddressRequestHandler : public RequestHandler {
private:
public:
    AddressRequestHandler(boost::asio::ip::udp::socket& socket, std::shared_ptr<Dhcp::Statistics> dhcpStatistics);

    bool Process(const Packet& packet) override;
};

class ReleaseRequestHandler : public RequestHandler {
private:
public:
    ReleaseRequestHandler(boost::asio::ip::udp::socket& socket, std::shared_ptr<Dhcp::Statistics> dhcpStatistics);

    bool Process(const Packet& packet) override;
};

} // namespace Dhcp