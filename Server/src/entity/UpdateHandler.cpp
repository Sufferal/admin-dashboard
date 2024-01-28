#include "../include/UpdateHandler.h"
#include "../include/AdminClient.h"
#include <algorithm>

using json = nlohmann::json;


UpdateHandler::UpdateHandler(const std::string& requestBody) : requestBody(requestBody){}

std::optional<std::string> getLatestVersionFilePath(const std::string& osType){
    std::string versionFileDirectory = "./Releases/";

    // Convert osType to uppercase
    std::string uppercaseOsType = osType;
    std::transform(uppercaseOsType.begin(), uppercaseOsType.end(), uppercaseOsType.begin(), ::toupper);

    // Ensure the Releases directory exists
    if (!std::filesystem::is_directory(versionFileDirectory)) {
        std::cout << "Releases directory not found!" << std::endl;
        return std::nullopt;
    }

    std::string jsonFilePath = versionFileDirectory + uppercaseOsType + "/LatestVersion.json";

    // Check if the JSON file exists
    if (!std::filesystem::exists(jsonFilePath)) {
        std::cout << "LatestVersion.json not found for OS type: " << uppercaseOsType << std::endl;
        return std::nullopt;
    }

    // Read the JSON file
    std::ifstream jsonFile(jsonFilePath);
    json jsonData;
    jsonFile >> jsonData;

    // Extract the latest version and filename from the JSON data
    std::string latestVersion = jsonData["latest_version"]["version"];
    std::string filename = jsonData["latest_version"]["filename"];

    // Construct the file path for the latest version file
    std::string versionFilePath = versionFileDirectory + uppercaseOsType + "/" + filename;

    // Check if the file exists
    if (!std::filesystem::exists(versionFilePath)) {
        std::cout << "File does not exist: " << versionFilePath << std::endl;
        return std::nullopt;
    }

    std::cout << "Latest version file path for OS type " << uppercaseOsType << ": " << versionFilePath << std::endl;
    return versionFilePath;
}

void UpdateHandler::handleUpdate(Poco::Net::HTTPServerResponse& response) const {
    // Set the response status code and message based on update success
    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
    std::ostream& ostr = response.send();

    std::vector<std::string> requestedIds = getIds();

    // Get the IDs, IP addresses, and OS types from the database
    DatabaseHandler& dbHandler = DatabaseSingleton::getInstance();
    MongoHandler getIPAndOSTypeMongo(dbHandler.getUriString(), dbHandler.getDatabase(), dbHandler.getCollectionClient());
    std::vector<std::string> ids = getIPAndOSTypeMongo.GetIDs();
    std::vector<std::string> ipAddresses = getIPAndOSTypeMongo.GetIpAddresses();
    std::vector<std::string> osTypes = getIPAndOSTypeMongo.GetOSTypes();

    // Find the matching IDs between the requested IDs and IDs from the database
    std::vector<std::string> matchingIds;
    std::vector<std::string> matchingOsTypes;
    for (const auto& requestedId : requestedIds) {
        auto it = std::find(ids.begin(), ids.end(), requestedId);
        if (it != ids.end()) {
            size_t index = std::distance(ids.begin(), it);
            matchingIds.push_back(requestedId);
            matchingOsTypes.push_back(osTypes[index]);
        }
    }

if (!matchingIds.empty()) {
        for (size_t i = 0; i < matchingIds.size(); ++i) {
            // Get the corresponding IP address and OS type for each matching ID
            auto it = std::find(ids.begin(), ids.end(), matchingIds[i]);
            if (it != ids.end()) {
                size_t index = std::distance(ids.begin(), it);
                if (index < ipAddresses.size()) {
                    std::string ipAddress = ipAddresses[index] + ":9090";
                    std::string osType = matchingOsTypes[i];

                    // Create an instance of AdminClient
                    auto channel = grpc::CreateChannel(ipAddress, grpc::InsecureChannelCredentials());
                    AdminClient client(channel);

                    // Get the latest version file path based on the OS type
                    std::optional<std::string> versionFilePath = getLatestVersionFilePath(osType);

                    if (versionFilePath.has_value()) {
                        ServiceCallResult updateResult = client.UpdateApplication(versionFilePath.value());
                        std::cout << "Sending update request to IP: " << ipAddress << std::endl;
                        std::cout << "Update result for IP " << ipAddress << ": " << updateResult << std::endl;
                    } else {
                        std::cout << "No valid version found for OS type: " << osType << " for IP: " << ipAddress << std::endl;
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
    }
}


std::vector<std::string> UpdateHandler::getIds() const {
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
