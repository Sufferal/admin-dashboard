#pragma once

#include <iostream>
#include <iostream>
#include <grpcpp/grpcpp.h>
#include "client_data_transmission.grpc.pb.h"


class DataInserterClientImpl
{
public:
    DataInserterClientImpl(std::shared_ptr<grpc::Channel> channel) : stub_(ClientDataInserterService::NewStub(channel)) {}

    ClientDataInserterResponse SendClientData(const ClientDataInserterRequest& request);

private:
    std::unique_ptr<ClientDataInserterService::Stub> stub_;
};