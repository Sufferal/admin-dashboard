#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/exception/exception.hpp>
#include <mongocxx/exception/logic_error.hpp>
#include "../include/DatabaseSingleton.h"

class LoginDb {
public:
    bool checkCredentials(const std::string& username, const std::string& password) const;

private:
     // Helper function to compute the SHA-256 hash of a string
    std::string sha256(const std::string& input) const;
};
