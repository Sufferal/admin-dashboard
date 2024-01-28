#pragma once

#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include "RequestHandler.h"

using namespace Poco::Net;

class RequestHandlerFactory : public HTTPRequestHandlerFactory
{
public:
   HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request) override;
};
