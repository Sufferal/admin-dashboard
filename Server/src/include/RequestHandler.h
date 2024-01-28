#pragma once

#include <iostream>
#include <fstream>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/StreamCopier.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Object.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/Path.h>
#include "../include/LoginRequestBody.h"
#include "../include/UpdateHandler.h"
#include "../include/RestartHandler.h"
#include "../include/ShutdownHandler.h"
#include "../include/DeviceHandler.h"

class RequestHandler : public Poco::Net::HTTPRequestHandler
{
  public:
      void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override;
      void serveStaticFile(const std::string& filePath, const std::string& contentType, Poco::Net::HTTPServerResponse& response);
};
