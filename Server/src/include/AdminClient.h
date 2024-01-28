#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "../../../Shared/proto/build/server_instructions.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

class AdminClient{
    public:
    AdminClient(std::shared_ptr<Channel> channel);
    ServiceCallResult Shutdown();
    ServiceCallResult Restart();
    ServiceCallResult UpdateApplication(const std::string& appInstallerPath);
    private:
    std::unique_ptr<AdminInstructions::Stub> stub_;
};