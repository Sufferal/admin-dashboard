#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPServerParams.h>
#include "RequestHandlerFactory.h"
#include <Poco/Util/ServerApplication.h>

class ServerHttp : public Poco::Util::ServerApplication
{
public:
    int main(const std::vector<std::string>& args) override;
};
