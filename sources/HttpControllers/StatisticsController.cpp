#include "StatisticsController.hpp"
#include <boost/json.hpp>
#include <vector>

namespace Statistics {

beast::http::response<beast::http::string_body> ResetRequest::operator()(beast::http::request<beast::http::string_body>& request,
                                                                         std::shared_ptr<Dhcp::Statistics> dhcpStatistics) {
    std::cout << "Reset there" << std::endl;
    beast::http::response<beast::http::string_body> response{beast::http::status::ok, request.version()};

    dhcpStatistics->Reset();

    return response;
}

beast::http::response<beast::http::string_body>
GetDroppedMessagesRequest::operator()(beast::http::request<beast::http::string_body>& request,
                                      std::shared_ptr<Dhcp::Statistics> dhcpStatistics) {
    std::cout << "Get dropped there" << std::endl;
    beast::http::response<beast::http::string_body> response{beast::http::status::ok, request.version()};
    auto droppedMessages = dhcpStatistics->GetDroppedMessages();
    boost::json::object jsonObject{};
    jsonObject["DroppedMessagesCount"] = droppedMessages.size();
    jsonObject["DroppedMessages"] = boost::json::value_from(droppedMessages);
//    std::string responseJson = boost::json::serialize(jsonObject);
//    response.body() = responseJson;

    return response;
}

} // namespace Statistics