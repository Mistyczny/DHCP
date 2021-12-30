#include "DhcpServer.hpp"
#include <boost/bind/bind.hpp>
#include <iostream>
#include <memory>

namespace Dhcp {

Server::Server(boost::asio::io_context& ioContext, Dhcp::Statistics& dhcpStatistics, AddressesCache& dhcpAddressesCache)
    : dhcpStatistics{dhcpStatistics}, dhcpAddressesCache{dhcpAddressesCache},
      socket{ioContext, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), DefaultServerPort)}, writeSocket{ioContext} {
    boost::asio::socket_base::broadcast option(true);
    this->socket.set_option(option);
    this->socket.set_option(boost::asio::ip::udp::socket::reuse_address(true));
    this->message.body.resize(DefaultMaxMessageSize);
    boost::system::error_code writeSocketError{};
    if(this->writeSocket.open(boost::asio::ip::udp::v4(), writeSocketError)) {
        std::cout<< "OPEN" << std::endl;
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
                this->dhcpStatistics.incrementDroppedMessages("Invalid action type: " +
                                                              std::to_string(static_cast<int>(dhcpPacket.GetPDU().operationCode)));
                break;
            }
        } catch (std::exception& ex) {
            std::cout << ex.what() << std::endl;
        }
    }

    this->Listen();
}

void Server::ProcessRequest(Packet& request) {
    for (auto& requestHandler : requestsHandlers) {
        if (requestHandler->Process(request)) {
            return;
        }
    }

    this->dhcpStatistics.incrementDroppedMessages("Invalid message type: " + std::to_string(request.GetMessageType()));
}

void Server::ProcessResponse(Packet& response) {
    this->dhcpStatistics.incrementDroppedMessages("Received message with response operation code");
}

} // namespace Dhcp