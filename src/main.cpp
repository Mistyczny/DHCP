#include "DhcpServer.hpp"
#include "../Source/Statistics/DhcpStatistics.hpp"
#include "DhcpAddressesCache.hpp"
#include "ManagementServer.hpp"
#include <iostream>

int main() {
    try {
        boost::asio::io_context ioContext{};
        boost::asio::signal_set signals(ioContext, SIGINT, SIGTERM);
        signals.async_wait([&](auto, auto){ ioContext.stop(); });

        auto dhcpStatistics = std::make_shared<Dhcp::Statistics>();

        Dhcp::AddressesCache dhcpAddressesCache{};

        auto dhcpServer = std::make_unique<Dhcp::Server>(ioContext, dhcpStatistics, dhcpAddressesCache);
        dhcpServer->Listen();
        auto managementServer = std::make_unique<Management::Server>(ioContext, dhcpStatistics);
        managementServer->Run();

        ioContext.run();
    } catch (std::exception& ex) {
        std::cout << "Exception: " << ex.what() << std::endl;
    }

    return 0;
}
