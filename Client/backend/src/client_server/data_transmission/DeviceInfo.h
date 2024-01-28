#pragma once

#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <optional>
#include "client_data_transmission.grpc.pb.h"

class DeviceInfo {
public:  
    static std::optional<std::string> getDeviceName();
    static OsType getOsType();
    static std::optional<std::string> getIpAddress();
    static std::optional<std::string> getGatewayInterface();
};
