#pragma once

#include <grpcpp/grpcpp.h>
#include "server_instructions.grpc.pb.h"
#include "../../include/UpdateConstants.h"

class AdminInstructionsImpl final : public AdminInstructions::Service
{
public:
    grpc::Status Shutdown(grpc::ServerContext* context, const ShutdownRequest* request, ShutdownResponse* response) override;

    grpc::Status Restart(grpc::ServerContext* context, const RestartRequest* request, RestartResponse* response) override;

    grpc::Status UpdateApplication(grpc::ServerContext* context, grpc::ServerReader<UpdateApplicationRequest>* reader, UpdateApplicationResponse* response) override;
};