#pragma once
#include <boost/beast.hpp>
#include <utility>

namespace beast = boost::beast;

template <class Send> class HttpControllerInterface {
protected:
    Send send;

public:
    explicit HttpControllerInterface(Send send) : send{send} {}
    ~HttpControllerInterface() = default;

    virtual bool Process(beast::http::request<beast::http::string_body>& request) = 0;
};
