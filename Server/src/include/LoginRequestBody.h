#pragma once

#include <string>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Object.h>
#include <Poco/Dynamic/Var.h>
#include "../../ESETServerSide.h"
#include "../include/LoginDb.h"

class LoginRequestBody {
public:
    LoginRequestBody(const std::string& requestBody);
    void handleLogin(Poco::Net::HTTPServerResponse& response) const;
    std::string getUsername() const;
    std::string getPassword() const;

private:
    std::string username;
    std::string password;
};
