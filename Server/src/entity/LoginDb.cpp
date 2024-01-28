#include "../include/LoginDb.h"

std::string LoginDb::sha256(const std::string& input) const {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, input.c_str(), input.length());
    SHA256_Final(hash, &sha256);

    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }

    return ss.str();
}

bool LoginDb::checkCredentials(const std::string& username, const std::string& password) const {
    try {
        DatabaseHandler& db_login = DatabaseSingleton::getInstance();
        auto coll = db_login.getClient()[db_login.getDatabase()][db_login.getCollectionConsole()];

        // Check if the username exists in the database
        bsoncxx::builder::stream::document filter_builder{};
        filter_builder << "username" << username;
        auto filter = filter_builder.view();
        auto result = coll.find_one(filter);

        if (result) {
            // Retrieve the password and salt from the database
            auto document_view = result.value().view();
            std::string db_password = document_view["password"].get_utf8().value.to_string();
            std::string salt = document_view["salt"].get_utf8().value.to_string();

            // Hash the provided password with the retrieved salt using SHA-256
            std::string hashed_password = sha256(password + salt);

            // Compare the hashed password with the password from the database
            if (hashed_password == db_password) {
                return true;
            } 
        } 
        
        // The username does not exist in the database or the password is incorrect
        return false;
    } catch (const std::exception& e) {
        std::cerr << "Error during database query: " << e.what() << std::endl;
        return false;
    }
}