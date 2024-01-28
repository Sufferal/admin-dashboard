#pragma once

#include <string>
#include <vector>
#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>

class MongoHandler {
public:
    MongoHandler(const std::string& connectionString, const std::string& databaseName, const std::string& collectionName);

    std::vector<std::string> GetIpAddresses();
    std::vector<std::string> GetOSTypes();
    std::vector<std::string> GetIDs();


private:
    std::vector<std::string> getInfoFromMongoDB(const std::string& dataType);

    mongocxx::client client_;
    mongocxx::collection collection_;
};
