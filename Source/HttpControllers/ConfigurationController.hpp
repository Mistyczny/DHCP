#pragma once
#include "Configuration.hpp"
#include "HttpControllerInterface.hpp"
#include <iostream>
#include <map>
#include <memory>
#include <regex>

namespace Configuration {

struct SetLoggingLevel {
    beast::http::response<beast::http::string_body> operator()(beast::http::request<beast::http::string_body>&,
                                                               std::shared_ptr<Configuration::Config>);
};

struct GetLoggingLevel {
    beast::http::response<beast::http::string_body> operator()(beast::http::request<beast::http::string_body>&,
                                                               std::shared_ptr<Configuration::Config>);
};

template <class Send> class Controller : public HttpControllerInterface<Send> {
private:
    std::shared_ptr<Configuration::Config> config;
    typedef std::pair<beast::http::verb, std::string> HttpMethod;
    typedef std::function<beast::http::response<beast::http::string_body>(beast::http::request<beast::http::string_body>&,
                                                                          std::shared_ptr<Configuration::Config>)>
        HttpRequestHandler;

    std::map<HttpMethod, HttpRequestHandler> controllers;

public:
    explicit Controller(Send send, std::shared_ptr<Configuration::Config> config)
        : HttpControllerInterface<Send>{send}, config{std::move(config)} {
        if (this->config == nullptr) {
            std::cout << "Controller config nullptr" << std::endl;
        }
        this->controllers.emplace(std::make_pair(beast::http::verb::post, R"((\/configuration\/logging\/level\/)(\d))"), SetLoggingLevel());
        this->controllers.emplace(std::make_pair(beast::http::verb::get, R"(\/configuration\/logging\/level)"), GetLoggingLevel());
    }

    bool Process(beast::http::request<beast::http::string_body>& request) override {
        std::cout << "Configuration controller" << std::endl;

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

        beast::http::response<beast::http::string_body> response;
        if (requestHandler != this->controllers.end()) {
            std::cout << "Found valid handler" << std::endl;
            response = requestHandler->second(request, this->config);
            std::cout << "Sending response" << std::endl;
        } else {
            response = beast::http::response<beast::http::string_body>{beast::http::status::not_found, request.version()};
            std::cout << "Not found valid handler" << std::endl;
        }
        this->send(std::move(response));

        std::cout << "Super" << std::endl;

        return true;
    }
};

} // namespace Configuration