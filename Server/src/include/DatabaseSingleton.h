#pragma once

#include <string>
#include "../include/DatabaseHandler.h"


class DatabaseSingleton {
public:
    static DatabaseHandler& getInstance();

private:

    static inline std::string uri_str = "mongodb+srv://sorin:xfbJceh1zzyoBLyA@esetdb.vkybcxb.mongodb.net/?retryWrites=true&w=majority";
    static inline std::string db_name = "ESET_Project";
    static inline std::string collection_client = "Client";
    static inline std::string collection_console = "Console";
};