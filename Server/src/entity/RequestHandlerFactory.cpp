#include "../include/RequestHandlerFactory.h"

Poco::Net::HTTPRequestHandler* RequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest& request)
{
  return new RequestHandler;
}
