#include "UpdateServiceImpl.h"
#include "../../client_client/manual_update/ManualUpdateClient.h"


grpc::Status UpdateServiceImpl::GetAppUpdate(grpc::ServerContext* context, const UpdateRequest* request, UpdateResponse* response) {
    
    std::string server_address = "intern.server:2048";
    std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials());
    ManualUpdateClient client(channel);

    if (client.PerformManualUpdate())
    {
        response->set_update_response(SERVICE_EXECUTED_SUCCESFULLY);
    }
    else
    {
        response->set_update_response(SERVICE_FAILED_TO_EXECUTE);
    }

    return grpc::Status::OK;
}