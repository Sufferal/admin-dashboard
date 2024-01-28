#include "../include/ServerHttp.h"

#define HTTP_SERVER_LISTENING_PORT_NUMBER 8080

int ServerHttp::main(const std::vector<std::string>&)
{
    Poco::Net::HTTPServer ServerHttp(new RequestHandlerFactory, HTTP_SERVER_LISTENING_PORT_NUMBER);

    ServerHttp.start();
    std::cout << "Server started on port 8080." << std::endl;

    waitForTerminationRequest();

    ServerHttp.stop();
    std::cout << "Server stopped." << std::endl;

    return Poco::Util::Application::EXIT_OK;
}
