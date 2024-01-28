#include "../include/DatabaseHandler.h"

DatabaseHandler::DatabaseHandler(const std::string& uri_str, const std::string& database, const std::string& collection_client, const std::string& collection_console)
    : uri_str(uri_str), database_name(database), collection_client_name(collection_client), collection_console_name(collection_console), client(mongocxx::uri{uri_str}) {}

bool DatabaseHandler::connect() {
    return static_cast<bool>(client);
}

void DatabaseHandler::disconnect() {
}

bool DatabaseHandler::insertDocument(const std::string& database, const std::string& collection, const bsoncxx::document::view& document) {
    try {
        auto coll = client[database][collection];
        coll.insert_one(document);
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

mongocxx::client& DatabaseHandler::getClient() {
    return client;
}

std::string DatabaseHandler::getDatabase() {
    return database_name;
}

std::string DatabaseHandler::getCollectionClient() {
    return collection_client_name;
}

std::string DatabaseHandler::getCollectionConsole() {
    return collection_console_name;
}

std::string DatabaseHandler::getUriString(){
    return uri_str;
}

std::string DatabaseHandler::queryAll(const std::string& database, const std::string& collection) {
    try {
        std::string jsonStrResult = "[";
        auto coll = client[database][collection];
        mongocxx::cursor cursor = coll.find({});

        for (auto doc : cursor) {
            jsonStrResult.append(bsoncxx::to_json(doc) + ",");
        }

        if (jsonStrResult.length() > 2){
            jsonStrResult.erase(jsonStrResult.length()-1);
        }        
        
        jsonStrResult.append("]");
        return jsonStrResult; 

    } catch (const std::exception& e) {
        std::cerr << "Error fetching data from the database: " << e.what() << std::endl;
        return "{}";
    }
}

