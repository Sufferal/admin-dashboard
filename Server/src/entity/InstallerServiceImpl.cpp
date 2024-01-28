#include "../include/InstallerServiceImpl.h"
#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

grpc::Status ManualUpdateServiceImpl::PerformManualUpdate(grpc::ServerContext* context, const ManualUpdateRequest* request, grpc::ServerWriter<ManualUpdateResponse>* writer) {
    // Get the latest version file path based on the OS type
    std::optional<std::string> versionFilePathOpt = getLatestVersion(request->os());

    if (!versionFilePathOpt) {
        return grpc::Status(grpc::StatusCode::NOT_FOUND, "No valid installer found for OS type");
    }

    std::string versionFilePath = *versionFilePathOpt;
    std::cout << "Requested OS:" << versionFilePath << std::endl;

    // Read the installer file content
    std::ifstream installerFile(versionFilePath, std::ios::binary);
    if (!installerFile) {
        return grpc::Status(grpc::StatusCode::NOT_FOUND, "Installer file not found");
    }

    // Set the chunk size to 1 MB
    constexpr int chunkSize = 1024 * 1024;
    char buffer[chunkSize];

    // Read and send the file in chunks
    while (!installerFile.eof()) {
        installerFile.read(buffer, chunkSize);
        std::streamsize bytesRead = installerFile.gcount();

        if (bytesRead > 0) {
            ManualUpdateResponse response;
            response.set_update_installer(buffer, bytesRead);
            if (!writer->Write(response)) {
                return grpc::Status(grpc::StatusCode::INTERNAL, "Failed to write response to client");
            }
        }
    }

    return grpc::Status::OK;
}

std::optional<std::string> ManualUpdateServiceImpl::getLatestVersion(OsType osType) {

    std::filesystem::path versionFileDirectory("Releases");

    auto latestVersionFilePath = versionFileDirectory / OsType_Name(osType) / "LatestVersion.json";

    // Open the JSON file, parse and check for latest
    std::ifstream jsonFile(latestVersionFilePath);
    if (!jsonFile.is_open()) {
        std::cout << "Error opening the LatestVersion JSON file: " << latestVersionFilePath << std::endl;
        return std::nullopt;
    }

    nlohmann::json jsonData;
    try {
        jsonFile >> jsonData;
    } catch (const std::exception& e) {
        std::cout << "Error parsing JSON content: " << e.what() << std::endl;
        return std::nullopt;
    }

    if (!jsonData.contains("latest_version")) {
        std::cout << "Invalid JSON format: missing \"latest_version\" field" << std::endl;
        return std::nullopt;
    }

    nlohmann::json latestVersionData = jsonData["latest_version"];

    // Check if the "latest_version" object has "version" and "filename" fields
    if (!latestVersionData.contains("version") || !latestVersionData.contains("filename")) {
        std::cout << "Invalid JSON format: missing \"version\" or \"filename\" field" << std::endl;
        return std::nullopt;
    }

    // Get the version and filename from the JSON
    std::string version = latestVersionData["version"].get<std::string>();
    std::string filename = latestVersionData["filename"].get<std::string>();

    auto installerFilePath = versionFileDirectory / OsType_Name(osType) / filename;

    // Check if the installer file exists
    if (!std::filesystem::exists(installerFilePath)) {
        std::cout << "Installer file not found: " << installerFilePath << std::endl;
        return std::nullopt;
    }

    std::cout << "Latest version: " << version << std::endl;
    std::cout << "Installer file path: " << installerFilePath << std::endl;

    return installerFilePath;
}