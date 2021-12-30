#include "StatisticsController.hpp"

namespace Statistics {

void ResetRequested::operator()(boost::beast::http::request<boost::beast::http::string_body>&) {
    std::cout << "From there" << std::endl;
}

} // namespace Statistics