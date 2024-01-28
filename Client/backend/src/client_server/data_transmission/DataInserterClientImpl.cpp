#include "DataInserterClientImpl.h"

ClientDataInserterResponse DataInserterClientImpl::SendClientData(const ClientDataInserterRequest& request) {
    ClientDataInserterResponse response;
    grpc::ClientContext context;

    grpc::Status status = stub_->SendClientData(&context, request, &response);

    if (!status.ok()) {
        std::cout << "RPC failed: " << status.error_message() << std::endl;
    }

    return response;
}

