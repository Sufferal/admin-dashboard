#include "../include/RequestHandler.h"

std::unordered_map<std::string, std::string> contentTypeMap = {
  { "html", "text/html" },
  { "css", "text/css" },
  { "js", "application/javascript" },
  { "png", "image/png" },
  { "jpg", "image/jpeg" },
};

void RequestHandler::serveStaticFile(const std::string& filePath, const std::string& contentType, Poco::Net::HTTPServerResponse& response) {
  std::ifstream file(filePath, std::ios::in | std::ios::binary);
  if (file.is_open()) {
    std::ostringstream contentStream;
    contentStream << file.rdbuf();
    std::string content = contentStream.str();
    response.setContentType(contentType);
    response.sendBuffer(content.data(), content.size());
  } else {
    std::cerr << "Failed to open file: " << filePath << std::endl;
    response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
    response.send();
  }
}


void RequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
  // Set CORS headers
  response.set("Access-Control-Allow-Origin", "*");
  response.set("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
  response.set("Access-Control-Allow-Headers", "Content-Type");

  // Handle preflight requests (OPTIONS)
  if (request.getMethod() == "OPTIONS") {
    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
    response.send();
    return;
  }

  std::cout << "Request from " << request.clientAddress().toString() << std::endl;
  std::cout << "Request method: " << request.getMethod() << std::endl;
  std::cout << "Request URI: " << request.getURI() << std::endl;

  std::istream& requestBodyStream = request.stream();
  std::string requestBody;
  Poco::StreamCopier::copyToString(requestBodyStream, requestBody);
  std::cout << "Request body: " << requestBody << std::endl;

  response.setChunkedTransferEncoding(true);

  std::string uri = request.getURI();
  std::string filePath = "assets" + uri;

  // Serve login.html separately
  if (uri == "/") {
    serveStaticFile("assets/login.html", "text/html", response);
    return;
  }

  // Serve static files
  size_t extensionPos = uri.rfind('.');
  if (extensionPos != std::string::npos) {
    std::string fileExtension = uri.substr(extensionPos + 1);
    auto it = contentTypeMap.find(fileExtension);
    if (it != contentTypeMap.end()) {
      std::string contentType = it->second;
      serveStaticFile(filePath, contentType, response);
      return;
    }
  }

  response.setContentType("text/plain");

  // Handle API requests
  if (uri == "/login") {
    LoginRequestBody loginRequest(requestBody);
    loginRequest.handleLogin(response);
    return;
  } else if (uri == "/shutdown") {
    ShutdownHandler shutdownRequest(requestBody);
    shutdownRequest.handleShutdown(response);
    return;
  } else if (uri == "/restart") {
    RestartHandler restartRequest(requestBody);
    restartRequest.handleRestart(response);
    return;
  } else if (uri == "/update") {
    UpdateHandler updateRequest(requestBody);
    updateRequest.handleUpdate(response);
    return;
  } else if (uri == "/devices"){
    response.setContentType("application/json");
    DeviceHandler deviceRequest(requestBody);
    deviceRequest.handleDevice(response);
    return;
  }
}
