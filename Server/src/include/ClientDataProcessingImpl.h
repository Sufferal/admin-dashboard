#pragma once

#include <grpcpp/grpcpp.h>
#include "../../../Shared/proto/build/client_data_transmission.grpc.pb.h"
#include "../include/JsonLoader.h"

class ClientDataProcessingImpl final : public ClientDataInserterService::Service {
public:
    grpc::Status SendClientData(grpc::ServerContext* context, const ClientDataInserterRequest* request, ClientDataInserterResponse* response) override;
};