#include "DhcpRequestsHandlers.hpp"
#include "Message.hpp"

namespace Dhcp {

RequestHandler::RequestHandler(boost::asio::ip::udp::socket& socket, Statistics& dhcpStatistics)
    : socket{socket}, dhcpStatistics{dhcpStatistics} {}

void RequestHandler::sendResponse(PDU&& pdu) {
    std::cout << "Sending response" << std::endl;
    const boost::asio::ip::udp::endpoint broadcastEndpoint(boost::asio::ip::address_v4::broadcast(), 68);
    Message<PDU> messageToSend{};
    messageToSend << pdu;
    this->socket.send_to(boost::asio::buffer(messageToSend.body), broadcastEndpoint);
    std::cout << "Sending response done" << std::endl;
}

DiscoverRequestHandler::DiscoverRequestHandler(boost::asio::ip::udp::socket& socket, Statistics& dhcpStatistics)
    : RequestHandler{socket, dhcpStatistics} {}

bool DiscoverRequestHandler::Process(const Packet& packet) {
    if (packet.GetMessageType() != MessageType::Discover) {
        return false;
    }

    try {
        std::cout << "Processing discover" << std::endl;
        const PDU& dhcpRequest = packet.GetPDU();

        PDU responsePDU{};
        responsePDU.operationCode = OperationCode::Response;
        responsePDU.hardwareType = dhcpRequest.hardwareType;
        responsePDU.hardwareTypeLength = dhcpRequest.hardwareTypeLength;
        responsePDU.hops = dhcpRequest.hops;
        responsePDU.transactionID = dhcpRequest.transactionID;
        responsePDU.secondsElapsed = dhcpRequest.secondsElapsed;
        responsePDU.flags = 0x0000;
        responsePDU.hardwareAddress = dhcpRequest.hardwareAddress;

        this->sendResponse(std::move(responsePDU));
    } catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }

    return true;
}

AddressRequestHandler::AddressRequestHandler(udp::socket& socket, Statistics& dhcpStatistics) : RequestHandler(socket, dhcpStatistics) {}

bool AddressRequestHandler::Process(const Packet& packet) {
    if (packet.GetMessageType() != MessageType::Request) {
        return false;
    }

    try {
        std::cout << "Processing address request" << std::endl;
        const PDU& dhcpRequest = packet.GetPDU();

        PDU responsePDU{};
        responsePDU.operationCode = OperationCode::Response;
        responsePDU.hardwareType = dhcpRequest.hardwareType;
        responsePDU.hardwareTypeLength = dhcpRequest.hardwareTypeLength;
        responsePDU.hops = dhcpRequest.hops;
        responsePDU.transactionID = dhcpRequest.transactionID;
        responsePDU.secondsElapsed = dhcpRequest.secondsElapsed;
        responsePDU.flags = 0x0000;
        responsePDU.hardwareAddress = dhcpRequest.hardwareAddress;

        this->sendResponse(std::move(responsePDU));
    } catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }

    return true;
}

ReleaseRequestHandler::ReleaseRequestHandler(udp::socket& socket, Statistics& dhcpStatistics) : RequestHandler(socket, dhcpStatistics) {}

bool ReleaseRequestHandler::Process(const Packet& packet) {
    if (packet.GetMessageType() != MessageType::Release) {
        return false;
    }

    try {
        std::cout << "Processing address request" << std::endl;
        const PDU& dhcpRequest = packet.GetPDU();

        PDU responsePDU{};
        responsePDU.operationCode = OperationCode::Response;
        responsePDU.hardwareType = dhcpRequest.hardwareType;
        responsePDU.hardwareTypeLength = dhcpRequest.hardwareTypeLength;
        responsePDU.hops = dhcpRequest.hops;
        responsePDU.transactionID = dhcpRequest.transactionID;
        responsePDU.secondsElapsed = dhcpRequest.secondsElapsed;
        responsePDU.flags = 0x0000;
        responsePDU.hardwareAddress = dhcpRequest.hardwareAddress;

        this->sendResponse(std::move(responsePDU));
    } catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }

    return true;
}

} // namespace Dhcp