#pragma once
#include <boost/beast.hpp>
#include <iostream>
#include <map>
#include <ranges>
#include <regex>

namespace Statistics {

struct ResetRequested {
    void operator()(boost::beast::http::request<boost::beast::http::string_body>&);
};

template <class Send> class Controller {
private:
    Send send;

    typedef std::pair<boost::beast::http::verb, std::string> HttpMethod;
    typedef std::function<void(boost::beast::http::request<boost::beast::http::string_body>&)> HttpRequestHandler;

    struct Comparator {
        template <typename T> bool operator()(const T& l, const T& r) const {
            if (l.first == r.first) {
                return l.second > r.second;
            }

            return l.first < r.first;
        }
    };

    std::map<std::string, std::function<void(boost::beast::http::request<boost::beast::http::string_body>&)>> controllers;

    std::map<HttpMethod, HttpRequestHandler> cntrls;

public:
    explicit Controller(Send& send) : send{send} {
        //controllers.emplace("/statistics/reset", ResetRequested());
        cntrls.emplace(std::make_pair(boost::beast::http::verb::get, "/statistics/reset"), ResetRequested());
        cntrls.emplace(std::make_pair(boost::beast::http::verb::get, "/statistics/login/$value"), ResetRequested());
    }
    virtual ~Controller() = default;

    bool Process(boost::beast::http::request<boost::beast::http::string_body>& request) {
        if (!request.target().starts_with("/statistics")) {
            return false;
        }
        std::string target = request.target().data();
        std::string someStrgin =  "abcd";
        if(someStrgin == request.target()) {
        }
        std::cout << "DATA: " << target << std::endl;
        //        auto requestHandler = this->controllers.find([&request](auto& iter){
        //            return true;
        //        });
       // auto requestHandler = this->cntrls.find(std::make_pair(5, 5));
        //        auto requestHandler = std::find_if(this->cntrls.begin(), this->cntrls.end(), [](const HttpMethod & t){
        //            return true;
        //        });
//        if (requestHandler != this->cntrls.end()) {
//            std::cout << "Found valid handler" << std::endl;
//            // requestHandler->second()(requestHandler);
//        } else {
//            std::cout << "Not found valid handler" << std::endl;
//        }

        std::cout << "Super" << std::endl;

        return false;
    }
};

} // namespace Statistics