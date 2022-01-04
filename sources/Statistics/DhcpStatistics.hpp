#pragma once
#include "DhcpPacket.hpp"
#include <cstdint>
#include <string>
#include <utility>
#include <list>

namespace Dhcp {

struct DroppedDhcpMessagesInterface {
    std::string errorMessage{};

    explicit DroppedDhcpMessagesInterface(std::string errorMessage);
    DroppedDhcpMessagesInterface(const DroppedDhcpMessagesInterface&) = default;
    DroppedDhcpMessagesInterface(DroppedDhcpMessagesInterface&&) = default;
    DroppedDhcpMessagesInterface& operator=(const DroppedDhcpMessagesInterface&) = default;
    DroppedDhcpMessagesInterface& operator=(DroppedDhcpMessagesInterface&&) = default;
    virtual ~DroppedDhcpMessagesInterface() = default;

    [[nodiscard]] virtual std::string ToString() const;
};

struct InvalidDhcpMessageOperationCode : public DroppedDhcpMessagesInterface {
    uint32_t invalidDhcpOperationCode;
    explicit InvalidDhcpMessageOperationCode(uint32_t dhcpOperationCode);
    [[nodiscard]] std::string ToString() const override;
};

struct InvalidDhcpMessageRequestType : public DroppedDhcpMessagesInterface {
    uint32_t invalidDhcpRequestType;
    explicit InvalidDhcpMessageRequestType(uint32_t invalidDhcpRequestType);
    [[nodiscard]] std::string ToString() const override;
};

class Statistics {
private:
    std::list<std::unique_ptr<DroppedDhcpMessagesInterface>> droppedDhcpMessages;
    uint64_t droppedMessages = 0;

public:
    Statistics() = default;
    ~Statistics() = default;
    Statistics(const Statistics&) = delete;
    Statistics& operator=(const Statistics& other) = delete;
    Statistics(Statistics&&) = delete;
    Statistics& operator=(const Statistics&& other) = delete;

    void IncrementDroppedMessages(const std::string&);
    void IncrementDroppedMessages(std::unique_ptr<DroppedDhcpMessagesInterface>);
    [[nodiscard]] uint64_t GetDroppedMessagesCount() const;
    [[nodiscard]] std::list<std::string> GetDroppedMessages() const;
    void Reset();
};

} // namespace Dhcp