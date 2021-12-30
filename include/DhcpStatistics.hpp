#pragma once
#include <cstdint>
#include <string>

namespace Dhcp {

class Statistics {
private:
    uint32_t droppedMessages = 0;

public:
    Statistics() = default;
    ~Statistics() = default;
    Statistics(const Statistics&) = delete;
    Statistics& operator=(const Statistics& other) = delete;
    Statistics(Statistics&&) = delete;
    Statistics& operator=(const Statistics&& other) = delete;

    void incrementDroppedMessages(const std::string&);
};

}