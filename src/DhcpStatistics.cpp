#include "DhcpStatistics.hpp"
#include <iostream>
#include <chrono>

namespace Dhcp {

void Statistics::incrementDroppedMessages(const std::string& error) {
    auto time = std::chrono::steady_clock::now();
    std::cout << time.time_since_epoch().count() << ": Dropped message with error: " << error << std::endl;
    this->droppedMessages += 1;
}

} // namespace Dhcp