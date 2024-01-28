#include "../ManualUpdateClient.h"
#include <fstream>
#include <Poco/Process.h>
#include <Poco/Exception.h>

ManualUpdateClient::ManualUpdateClient(std::shared_ptr<grpc::Channel> channel)
        : stub_(ManualUpdateService::NewStub(channel)) {}

bool ManualUpdateClient::PerformManualUpdate() {
    ManualUpdateRequest request;
    request.set_os(WINDOWS);

    grpc::ClientContext context;
    std::unique_ptr<grpc::ClientReader<ManualUpdateResponse>> reader(stub_->PerformManualUpdate(&context, request));

    std::ofstream outfile(UpdateConstants::WINDOWS_UPDATE_FILE_NAME, std::ofstream::binary);

    ManualUpdateResponse installer_chunk;
    while (reader->Read(&installer_chunk))
    {
        outfile.write(installer_chunk.update_installer().data(), installer_chunk.update_installer().size());
    }
    
    try
    {
        Poco::Process::launch("msiexec", { "/i", UpdateConstants::WINDOWS_UPDATE_FILE_NAME });
        return true;
    }
    catch (const Poco::Exception& ex)
    {
        std::cerr << "Error: " << ex.displayText() << std::endl;
        return false;
    }
}
