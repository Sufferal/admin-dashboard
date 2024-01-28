#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "DatabaseHandler.h"
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/array.hpp>
#include "../include/DatabaseSingleton.h"
#include "../include/DatabaseControls.h"
#include "../include/UpdateHandler.h"
#include "../../../Shared/proto/build/client_data_transmission.grpc.pb.h"


struct DeviceInfo {
    std::int32_t id;
    std::string device_name;
    std::string ip_address;
    std::string os_type;
    std::string app_version;
};

class JsonLoader {
public:
    bool insertJsonToDatabase(DeviceInfo deviceInfo);
    bool updateJsonInDatabase(DeviceInfo deviceInfo);
};