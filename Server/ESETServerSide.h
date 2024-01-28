// CMakeProject1.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Util/ServerApplication.h>
#include <Poco/StreamCopier.h>
#include "src/include/ServerHttp.h"
#include "src/include/JsonLoader.h"
#include "src/include/DatabaseSingleton.h"
#include "src/include/ClientDataProcessingImpl.h"
#include "src/include/IpAddress.h"
#include "src/include/InstallerServiceImpl.h"