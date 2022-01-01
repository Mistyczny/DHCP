#include "DhcpStatistics.hpp"
#include <chrono>
#include <iostream>

namespace Dhcp {

DroppedDhcpMessagesInterface::DroppedDhcpMessagesInterface(std::string errorMessage) : errorMessage(std::move(errorMessage)) {}

std::string DroppedDhcpMessagesInterface::ToString() const {
    std::string fullErrorMessage{};
    fullErrorMessage = this->errorMessage;
    return fullErrorMessage;
}

InvalidDhcpMessageOperationCode::InvalidDhcpMessageOperationCode(uint32_t invalidDhcpOperationCode)
    : DroppedDhcpMessagesInterface{"Invalid dhcp operation code: "}, invalidDhcpOperationCode{invalidDhcpOperationCode} {}

std::string InvalidDhcpMessageOperationCode::ToString() const {
    std::string fullErrorMessage{};
    fullErrorMessage = this->errorMessage;
    fullErrorMessage += std::to_string(this->invalidDhcpOperationCode);
    return fullErrorMessage;
}

void Statistics::IncrementDroppedMessages(const std::string& error) {
    this->droppedDhcpMessages.push_back(std::make_unique<DroppedDhcpMessagesInterface>(error));
    auto time = std::chrono::steady_clock::now();
    std::cout << time.time_since_epoch().count() << ": Dropped message with error: " << error << std::endl;
    this->droppedMessages += 1;
}

uint64_t Statistics::GetDroppedMessagesCount() const { return this->droppedMessages; }

void Statistics::Reset() {
    this->droppedMessages = 0;
    this->droppedDhcpMessages.clear();
}

void Statistics::IncrementDroppedMessages(std::unique_ptr<DroppedDhcpMessagesInterface> droppedDhcpMessage) {
    this->droppedDhcpMessages.push_back(std::move(droppedDhcpMessage));
}

std::list<std::string> Statistics::GetDroppedMessages() const {
    std::list<std::string> droppedMessagesList{};
    std::ranges::for_each(this->droppedDhcpMessages,
                          [&droppedMessagesList](const auto& iter) { droppedMessagesList.push_back(iter->ToString()); });
    return droppedMessagesList;
}

InvalidDhcpMessageRequestType::InvalidDhcpMessageRequestType(uint32_t invalidDhcpRequestType)
    : DroppedDhcpMessagesInterface("Invalid dhcp request type: "), invalidDhcpRequestType{invalidDhcpRequestType} {}

std::string InvalidDhcpMessageRequestType::ToString() const {
    std::string fullErrorMessage{};
    fullErrorMessage = this->errorMessage;
    fullErrorMessage += std::to_string(this->invalidDhcpRequestType);
    return fullErrorMessage;
}

} // namespace Dhcp