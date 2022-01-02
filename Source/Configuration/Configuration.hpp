#pragma once
#include <cstdint>
#include <string>

namespace Configuration {

struct DhcpServer {
    uint16_t port;
};

struct Logging {
    uint8_t level;
    std::string path;
};

struct Config {
    DhcpServer dhcpServerConfiguration{};
    Logging loggingConfiguration{};
};

} // namespace Configuration