#include "ConfigurationController.hpp"
#include <boost/json.hpp>

namespace Configuration {

beast::http::response<beast::http::string_body> SetLoggingLevel::operator()(beast::http::request<beast::http::string_body>& request,
                                                                            std::shared_ptr<Configuration::Config> config) {
    beast::http::response<beast::http::string_body> response{beast::http::status::not_acceptable, request.version()};
    std::cout << "From there is now working" << std::endl;
    const std::regex baseRegex(R"((\/configuration\/logging\/level\/)(\d))");
    std::cout << "From there is now working 1" << std::endl;
    std::smatch baseMatch;
    std::cout << "From there is now working 2" << std::endl;
    std::string requestTarget = request.target().to_string();
    std::cout << "From there is now working 3" << std::endl;

    if (std::regex_match(requestTarget, baseMatch, baseRegex)) {
        std::cout << "here" << std::endl;
        uint8_t newLoggingLevel = std::stoul(baseMatch[2]);
        if(!config) {
            std::cout << "Config is nullptr" << std::endl;
        } else if(newLoggingLevel < 1 || newLoggingLevel > 6) {
            std::string errorMessage{"Logging level : " + std::to_string((unsigned int)newLoggingLevel) + " is out of scope"};
            std::cout << errorMessage << std::endl;
            response.body() = errorMessage;
        } else {
            std::cout << "Setting logging level to new value: " << (unsigned int)newLoggingLevel << std::endl;
            config->loggingConfiguration.level = newLoggingLevel;
            response.body() = "New logging level value has been set";
        }
    } else {
        std::cout << "Regex failed" << std::endl;
    }

    return response;
}

beast::http::response<beast::http::string_body> GetLoggingLevel::operator()(beast::http::request<beast::http::string_body>& request,
                                                                            std::shared_ptr<Configuration::Config> configuration) {
    std::cout<< "Getting logging level" << std::endl;
    beast::http::response<beast::http::string_body> response{beast::http::status::ok, request.version()};
    boost::json::object jsonObject{};
    jsonObject["LoggingLevel"] = configuration->loggingConfiguration.level;
    //std::string responseJson = boost::json::serialize(jsonObject);
    //response.body() = responseJson;
    return response;
}
} // namespace Configuration