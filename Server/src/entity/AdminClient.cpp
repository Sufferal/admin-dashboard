#include "../include/AdminClient.h"
#include <fstream>

AdminClient::AdminClient(std::shared_ptr<Channel> channel)
    : stub_(AdminInstructions::NewStub(channel)) {}

ServiceCallResult AdminClient::Shutdown() {
    ShutdownRequest request;
    ShutdownResponse response;
    ClientContext context;

    Status status = stub_->Shutdown(&context, request, &response);
    if (status.ok()) {
        return response.shutdown_service_result();
    } else {
        std::cerr << "RPC failed: " << status.error_message() << std::endl;
        return ServiceCallResult::SERVICE_FAILED_TO_EXECUTE;
    }
}

ServiceCallResult AdminClient::Restart() {
    RestartRequest request;
    RestartResponse response;
    ClientContext context;

    Status status = stub_->Restart(&context, request, &response);
    if (status.ok()) {
        return response.restart_service_result();
    } else {
        std::cerr << "RPC failed: " << status.error_message() << std::endl;
        return ServiceCallResult::SERVICE_FAILED_TO_EXECUTE;
    }
}

ServiceCallResult AdminClient::UpdateApplication(const std::string& appInstallerPath) {
    UpdateApplicationRequest request;
    UpdateApplicationResponse response;
    ClientContext context;

    // Load app installer binary from the provided file path
    std::ifstream appInstallerFile(appInstallerPath, std::ios::binary | std::ios::ate);
    if (!appInstallerFile.is_open()) {
        std::cerr << "Failed to open app installer file: " << appInstallerPath << std::endl;
        return ServiceCallResult::SERVICE_FAILED_TO_EXECUTE;
    }
    int64_t fileSize = appInstallerFile.tellg();
    appInstallerFile.seekg(0, std::ios::beg);

    // Create a stream writer to send the data in chunks
    std::unique_ptr<grpc::ClientWriter<UpdateApplicationRequest>> writer(stub_->UpdateApplication(&context, &response));

    // Send the app installer data in chunks
    const int chunkSize = 1024 * 1024; // 1 MB chunk size
    std::vector<char> buffer(chunkSize);

    while (fileSize > 0) {
        int bytesRead = static_cast<int>(appInstallerFile.readsome(buffer.data(), chunkSize));
        if (bytesRead > 0) {
            request.set_app_installer(buffer.data(), bytesRead);

            if (!writer->Write(request)) {
                std::cerr << "Failed to send app installer chunk" << std::endl;
                return ServiceCallResult::SERVICE_FAILED_TO_EXECUTE;
            }
            fileSize -= bytesRead;
        } else if(bytesRead == 0){
            break; // No mare data available, break the loop
        } else{
            std::cerr << "Error reading app installer file " << std::endl;
            return ServiceCallResult::SERVICE_FAILED_TO_EXECUTE;
        }
    }

    writer->WritesDone();// signal the end of the stream to the server

    Status status = writer->Finish();
    if (status.ok()) {
        return response.update_application_service_result();
    } else {
        std::cerr << "RPC failed: " << status.error_message() << std::endl;
        return ServiceCallResult::SERVICE_FAILED_TO_EXECUTE;
    }
}
