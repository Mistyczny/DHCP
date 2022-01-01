#pragma once
#include "HttpControllerInterface.hpp"
#include <iostream>
#include <regex>

namespace Configuration {

template <class Send> class Controller : public HttpControllerInterface<Send> {
protected:
public:
    explicit Controller(Send send) : HttpControllerInterface<Send>{send} {}

    bool Process(beast::http::request<beast::http::string_body>& request) override {
        std::cout << "Configuration controller" << std::endl;
//        if (!request.target().starts_with("/statistics")) {
//            return false;
//        }
//
//        auto requestHandler = std::ranges::find_if(this->controllers, [&](const auto& iter) {
//            const auto& [key, value] = iter;
//            const std::regex baseRegex(key.second);
//            std::smatch baseMatch;
//            std::string target{request.target()};
//
//            if (key.first != request.method() || !std::regex_match(target, baseMatch, baseRegex)) {
//                return false;
//            } else {
//                return true;
//            }
//        });
//
//        if (requestHandler != this->controllers.end()) {
//            std::cout << "Found valid handler" << std::endl;
//            auto response = requestHandler->second(request, this->dhcpStatistics);
//            std::cout << "Sending response" << std::endl;
//            this->send(std::move(response));
//        } else {
//            std::cout << "Not found valid handler" << std::endl;
//        }
//
//        std::cout << "Super" << std::endl;

        return true;
    }
};

} // namespace Configuration