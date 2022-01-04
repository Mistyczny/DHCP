#pragma once
#include "DhcpStatistics.hpp"
#include "HttpControllerInterface.hpp"
#include <boost/beast.hpp>
#include <iostream>
#include <map>
#include <ranges>
#include <regex>

namespace beast = boost::beast;

namespace Statistics {

struct ResetRequest {
    beast::http::response<beast::http::string_body> operator()(beast::http::request<beast::http::string_body>&,
                                                               std::shared_ptr<Dhcp::Statistics> dhcpStatistics);
};

struct GetDroppedMessagesRequest {
    beast::http::response<beast::http::string_body> operator()(beast::http::request<beast::http::string_body>&,
                                                               std::shared_ptr<Dhcp::Statistics> dhcpStatistics);
};

template <class Send> class Controller : public HttpControllerInterface<Send> {
private:
    std::shared_ptr<Dhcp::Statistics> dhcpStatistics;

    typedef std::pair<beast::http::verb, std::string> HttpMethod;
    typedef std::function<beast::http::response<beast::http::string_body>(beast::http::request<beast::http::string_body>&,
                                                                          std::shared_ptr<Dhcp::Statistics> dhcpStatistics)>
        HttpRequestHandler;

    std::map<HttpMethod, HttpRequestHandler> controllers;

public:
    Controller(Send send, std::shared_ptr<Dhcp::Statistics> dhcpStatistics)
        : HttpControllerInterface<Send>{send}, dhcpStatistics{std::move(dhcpStatistics)} {
        controllers.emplace(std::make_pair(beast::http::verb::post, "\\/statistics\\/reset"), ResetRequest());
        controllers.emplace(std::make_pair(beast::http::verb::get, "\\/statistics\\/dropped"), GetDroppedMessagesRequest());
    }
    virtual ~Controller() = default;

    bool Process(beast::http::request<beast::http::string_body>& request) override {
        std::cout << "Statistics controller" << std::endl;

        auto requestHandler = std::ranges::find_if(this->controllers, [&](const auto& iter) {
            const auto& [key, value] = iter;
            const std::regex baseRegex(key.second);
            std::smatch baseMatch;
            std::string target{request.target()};

            if (key.first != request.method() || !std::regex_match(target, baseMatch, baseRegex)) {
                return false;
            } else {
                return true;
            }
        });

        if (requestHandler != this->controllers.end()) {
            std::cout << "Found valid handler" << std::endl;
            auto response = requestHandler->second(request, this->dhcpStatistics);
            std::cout << "Sending response" << std::endl;
            this->send(std::move(response));
        } else {
            std::cout << "Not found valid handler" << std::endl;
        }

        std::cout << "Super" << std::endl;

        return true;
    }
};

} // namespace Statistics