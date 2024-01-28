#pragma once

#include <string>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>

class DatabaseHandler {
public:
    DatabaseHandler(const std::string& uri_str, const std::string& database, const std::string& collection_client, const std::string& collection_console);
    bool connect();
    void disconnect();
    bool insertDocument(const std::string& database, const std::string& collection, const bsoncxx::document::view& document);
    std::string queryAll(const std::string& database, const std::string& collection);
    mongocxx::client& getClient();
    std::string getDatabase();
    std::string getCollectionClient();
    std::string getCollectionConsole();
    std::string getUriString();

private:
    std::string uri_str;
    std::string database_name = "";
    std::string collection_client_name = ""; 
    std::string collection_console_name = "";
    mongocxx::instance instance;
    mongocxx::client client;
};
