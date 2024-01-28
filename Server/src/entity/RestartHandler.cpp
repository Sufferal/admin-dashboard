#include "../include/RestartHandler.h"


RestartHandler::RestartHandler(const std::string& requestBody) : requestBody(requestBody){
}

void RestartHandler::handleRestart(Poco::Net::HTTPServerResponse& response) const {
    DatabaseControls dbControls;
    json data = json::parse(requestBody);

    for (const auto& item : data["_id"]) {
      int32_t id = std::stoi(item["_id"].get<std::string>());
      dbControls.handleRequest(DatabaseControls::RequestType::Shutdown, id);
    }

    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
    std::ostream& ostr = response.send();

    std::vector<std::string> requestedIds = getIds();

    // Get the IDs and IP addresses from the database
    DatabaseHandler& dbHandler = DatabaseSingleton::getInstance();
    MongoHandler getInfoFromMongoDB(dbHandler.getUriString(), dbHandler.getDatabase(), dbHandler.getCollectionClient());
    std::vector<std::string> ids = getInfoFromMongoDB.GetIDs();
    std::vector<std::string> ipAddresses = getInfoFromMongoDB.GetIpAddresses();

    // Print the IDs obtained from the database to check if they are correct
    std::cout << "IDs from the database: ";
    for (const auto& id : ids) {
        std::cout << id << " ";
    }
    std::cout << std::endl;

    // Find the matching IDs between the requested IDs and IDs from the database
    std::vector<std::string> matchingIds;
    for (const auto& requestedId : requestedIds) {
        auto it = std::find(ids.begin(), ids.end(), requestedId);
        if (it != ids.end()) {
            matchingIds.push_back(requestedId);
        }
    }

    if (!matchingIds.empty()) {
        for (const auto& matchingId : matchingIds) {
            // Get the corresponding IP address for each matching ID
            auto index = std::find(ids.begin(), ids.end(), matchingId) - ids.begin();
            if (index < ipAddresses.size()) {
                std::string ipAddress = ipAddresses[index] + ":9090";

                // Send the restart request to the corresponding IP address
                auto channel = grpc::CreateChannel(ipAddress, grpc::InsecureChannelCredentials());
                AdminClient client(channel);
                ServiceCallResult restartResult = client.Restart();
                std::cout << "Restart result for IP " << ipAddress << ": " << restartResult << std::endl;
            } else {
                std::cout << "No IP Address found in the collection for ID: " << matchingId << std::endl;
            }
        }
    } else {
        std::cout << "Requested IDs not found in the collection: ";
        for (const auto& requestedId : requestedIds) {
            std::cout << requestedId << " ";
        }
        std::cout << std::endl;
    }
}


std::vector<std::string> RestartHandler::getIds() const {
    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result = parser.parse(requestBody);
    Poco::JSON::Object::Ptr jsonObject = result.extract<Poco::JSON::Object::Ptr>();
    Poco::Dynamic::Var idVar = jsonObject->get("_id");
    std::string id = idVar.convert<std::string>();
    std::vector<std::string> numericIds;
    std::string numericId;
    for (char ch : id) {
        if (std::isdigit(ch)) {
            numericId += ch;

        } else {
            if (!numericId.empty()) {
                numericIds.push_back(numericId);
                numericId.clear();
            }
        }
    }
    if (!numericId.empty()) {
        numericIds.push_back(numericId);
    }
    return numericIds;
}
