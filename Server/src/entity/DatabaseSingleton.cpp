#include "../include/DatabaseSingleton.h"

DatabaseHandler& DatabaseSingleton::getInstance() {
    static DatabaseHandler instance(uri_str, db_name, collection_client, collection_console);
    return instance;
}
