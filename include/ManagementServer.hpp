#pragma once
#include <boost/asio.hpp>
#include "ManagementSession.hpp"
#include "DhcpStatistics.hpp"
#include "Configuration.hpp"
#include <boost/beast.hpp>
#include <memory>

namespace Management {

class Listener;

class Server {
private:
    boost::asio::io_context& ioContext;
    std::shared_ptr<Dhcp::Statistics> statistics;
    std::shared_ptr<Configuration::Config> configuration;
    std::shared_ptr<Listener> listener;

public:
    explicit Server(boost::asio::io_context&, std::shared_ptr<Configuration::Config>, std::shared_ptr<Dhcp::Statistics>);

    void Run();
};

}