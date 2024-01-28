#include "../include/DeviceHandler.h"

DeviceHandler::DeviceHandler(const std::string& requestBody){}

void DeviceHandler::handleDevice(Poco::Net::HTTPServerResponse& response) const {

    // Set the response status code and devices data
    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
    
    // Read the contents of the JSON file
    DatabaseHandler& db_devices = DatabaseSingleton::getInstance();
    std::string result = db_devices.queryAll(db_devices.getDatabase(), db_devices.getCollectionClient());

    // Set the appropriate HTTP headers
    response.setChunkedTransferEncoding(true);
    response.setContentType("application/json");

    // Set the response body to the JSON content
    std::ostream& responseBody = response.send();
    responseBody << result;
}