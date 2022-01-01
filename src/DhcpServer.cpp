#include "DhcpServer.hpp"
#include <boost/bind/bind.hpp>
#include <iostream>
#include <memory>

namespace Dhcp {

Server::Server(boost::asio::io_context& ioContext, std::shared_ptr<Dhcp::Statistics> dhcpStatistics, AddressesCache& dhcpAddressesCache)
    : dhcpStatistics{std::move(dhcpStatistics)}, dhcpAddressesCache{dhcpAddressesCache},
      socket{ioContext, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), DefaultServerPort)}, writeSocket{ioContext} {
    boost::asio::socket_base::broadcast option(true);
    this->socket.set_option(option);
    this->socket.set_option(boost::asio::ip::udp::socket::reuse_address(true));
    this->message.body.resize(DefaultMaxMessageSize);
    boost::system::error_code writeSocketError{};
    if (this->writeSocket.open(boost::asio::ip::udp::v4(), writeSocketError)) {
        std::cout << "OPEN" << std::endl;
    }
    this->writeSocket.set_option(boost::asio::ip::udp::socket::reuse_address(true));
    this->writeSocket.set_option(boost::asio::socket_base::broadcast(true));

    this->requestsHandlers.push_back(std::make_unique<DiscoverRequestHandler>(writeSocket, dhcpStatistics));
    this->requestsHandlers.push_back(std::make_unique<AddressRequestHandler>(writeSocket, dhcpStatistics));
    this->requestsHandlers.push_back(std::make_unique<ReleaseRequestHandler>(writeSocket, dhcpStatistics));
}

Server::~Server() { std::cout << "SERVER DEALLOCATED" << std::endl; }

void Server::Listen() {
    std::cout << "SIZE: " << this->message.body.size() << std::endl;
    std::fill(this->message.body.begin(), this->message.body.end(), '\0');
    this->socket.async_receive_from(
        boost::asio::buffer(this->message.body), remoteEndpoint,
        boost::bind(&Server::HandleReceive, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void Server::HandleReceive(const boost::system::error_code& error, std::size_t bytes) {
    if (error) {
        std::cout << error.message() << std::endl;
    } else if (bytes <= 0) {
        std::cout << "size <= 0" << std::endl;
    } else {
        try {
            PDU dhcpPDU{};
            this->message >> dhcpPDU;
            Packet dhcpPacket{};
            dhcpPacket.setPDU(std::move(dhcpPDU));

            switch (dhcpPacket.GetPDU().operationCode) {
            case OperationCode::Request:
                this->ProcessRequest(dhcpPacket);
                break;
            case OperationCode::Response:
                this->ProcessResponse(dhcpPacket);
                break;
            default:
                const auto invalidOperationCode = static_cast<const uint32_t>(dhcpPacket.GetPDU().operationCode);
                this->dhcpStatistics->IncrementDroppedMessages(std::make_unique<InvalidDhcpMessageRequestType>(invalidOperationCode));
                break;
            }
        } catch (std::exception& ex) {
            std::cout << ex.what() << std::endl;
        }
    }

    this->Listen();
}

void Server::ProcessRequest(Packet& request) {
    // Loop until we find valid request handler
    for (auto& requestHandler : requestsHandlers) {
        if (requestHandler->Process(request)) {
            return;
        }
    }

    auto operationCode = static_cast<uint64_t>(request.GetPDU().operationCode);
    this->dhcpStatistics->IncrementDroppedMessages(std::make_unique<InvalidDhcpMessageOperationCode>(operationCode));
}

void Server::ProcessResponse(Packet& response) {
    this->dhcpStatistics->IncrementDroppedMessages("Received message with response operation code");
}

} // namespace Dhcp