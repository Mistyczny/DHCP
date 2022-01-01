#pragma once
#include <boost/asio.hpp>
#include "ManagementSession.hpp"
#include "../Source/Statistics/DhcpStatistics.hpp"
#include <boost/beast.hpp>
#include <memory>

namespace Management {

class Listener;

class Server {
private:
    boost::asio::io_context& ioContext;
    std::shared_ptr<Dhcp::Statistics> dhcpStatistics;
    std::shared_ptr<Listener> listener;

public:
    explicit Server(boost::asio::io_context& ioContext, std::shared_ptr<Dhcp::Statistics> dhcpStatistics);

    void Run();
};

}