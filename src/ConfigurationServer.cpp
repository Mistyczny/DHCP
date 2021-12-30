#include "ConfigurationServer.hpp"

namespace Configuration {

Server::Server(boost::asio::io_context& ioContext)
    : ioContext{ioContext}, socket{ioContext, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), DefaultServerPort)} {}

void Server::Listen() {}

} // namespace Configuration