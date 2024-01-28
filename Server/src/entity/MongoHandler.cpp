#include "../include/MongoHandler.h"
#include <mongocxx/instance.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

MongoHandler::MongoHandler(const std::string& connectionString, const std::string& databaseName, const std::string& collectionName)
    : client_(mongocxx::uri(connectionString))
    , collection_(client_[databaseName][collectionName]) {}

std::vector<std::string> MongoHandler::GetIpAddresses() {
    return getInfoFromMongoDB("IP Address");
}

std::vector<std::string> MongoHandler::GetOSTypes() {
    return getInfoFromMongoDB("OS");
}

std::vector<std::string> MongoHandler::GetIDs() {
    return getInfoFromMongoDB("_id");
}

std::vector<std::string> MongoHandler::getInfoFromMongoDB(const std::string& dataType) {
    auto filter = bsoncxx::builder::basic::make_document();
    auto cursor = collection_.find(filter.view());
    std::vector<std::string> results;

    for (const auto& document : cursor) {
        bsoncxx::document::view view = document;
        auto it = view.find(dataType);
        if (dataType == "_id") {
            // Handle _id separately and convert it to string
            if (it != view.end()) {
                if (it->type() == bsoncxx::type::k_int32) {
                    int idValue = it->get_int32().value;
                    results.push_back(std::to_string(idValue));
                } else if (it->type() == bsoncxx::type::k_oid) {
                    bsoncxx::oid oidValue = it->get_oid().value;
                    results.push_back(oidValue.to_string());
                }
            }
        } else if (it != view.end() && it->type() == bsoncxx::type::k_utf8) {
            std::string value = it->get_utf8().value.to_string();
            results.push_back(value);
        }
    }

    return results;
}