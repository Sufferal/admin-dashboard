#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Object.h>
#include <Poco/Dynamic/Var.h>
#include "../include/DatabaseSingleton.h"

class DeviceHandler {
public:
    DeviceHandler(const std::string& requestBody);
    void handleDevice(Poco::Net::HTTPServerResponse& response) const;
};