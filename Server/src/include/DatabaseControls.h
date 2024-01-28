#pragma once

#include <string>
#include <iostream>
#include <bsoncxx/builder/stream/document.hpp>
#include <nlohmann/json.hpp>
#include <optional>
#include "../include/DatabaseSingleton.h"

using json = nlohmann::json;


class DatabaseControls {
public:
    enum class RequestType {
        Unspecified,
        Shutdown,
        Restart
    };
    enum class StatusType {
        Unspecified,
        Online,
        Offline
    };
    void handleRequest(const RequestType type, const std::int32_t& deviceId); 
    std::optional<uint32_t> idFind();
    std::optional<int32_t> findExistingIp(const std::string& ip_address);  
};
