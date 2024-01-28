#pragma once

#include <iostream>
#include <grpcpp/grpcpp.h>
#include "manual_update_service.grpc.pb.h"
#include "../../include/UpdateConstants.h"


class ManualUpdateClient {
public:
    ManualUpdateClient(std::shared_ptr<grpc::Channel> channel);

    bool PerformManualUpdate();

private:
    std::unique_ptr<ManualUpdateService::Stub> stub_;
};