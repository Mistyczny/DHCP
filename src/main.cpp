#include "DhcpAddressesCache.hpp"
#include "DhcpServer.hpp"
#include "DhcpStatistics.hpp"
#include "ManagementServer.hpp"
#include "Configuration.hpp"
#include <iostream>

int main() {
    try {
        boost::asio::io_context ioContext{};
        boost::asio::signal_set signals(ioContext, SIGINT, SIGTERM);
        signals.async_wait([&](auto, auto) { ioContext.stop(); });

        auto configuration = std::make_shared<Configuration::Config>();
        auto statistics = std::make_shared<Dhcp::Statistics>();

        if(configuration == nullptr) {
            std::cout << "Controller config nullptr" << std::endl;
        }

        Dhcp::AddressesCache dhcpAddressesCache{};
        auto addressesCache = std::make_unique<Dhcp::AddressesCache>();

        auto dhcpServer = std::make_unique<Dhcp::Server>(ioContext, statistics, dhcpAddressesCache);
        dhcpServer->Listen();
        auto managementServer = std::make_unique<Management::Server>(ioContext, configuration, statistics);
        managementServer->Run();

        ioContext.run();
    } catch (std::exception& ex) {
        std::cout << "Exception: " << ex.what() << std::endl;
    }

    return 0;
}
