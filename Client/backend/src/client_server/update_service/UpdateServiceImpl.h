#pragma once

#include <grpcpp/grpcpp.h>
#include "gui_communication.grpc.pb.h"

class UpdateServiceImpl final : public UpdateService::Service
{
public:
	grpc::Status GetAppUpdate(grpc::ServerContext* context, const UpdateRequest* request, UpdateResponse* response) override;
};