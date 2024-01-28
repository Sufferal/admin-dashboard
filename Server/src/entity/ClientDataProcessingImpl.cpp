#include "../include/ClientDataProcessingImpl.h"

grpc::Status ClientDataProcessingImpl::SendClientData(grpc::ServerContext* context, const ClientDataInserterRequest* request, ClientDataInserterResponse* response){
    
    JsonLoader json_loader;
    DeviceInfo device_info;

    switch(request->os_type()) {
        case OsType::LINUX:
            device_info.os_type = "Linux";
            break;

        case OsType::WINDOWS:
            device_info.os_type = "Windows";
            break;

        default:
            device_info.os_type = "Unspecified";
            break;
	}

    device_info.device_name = request->device_name();
    device_info.ip_address = request->ip_address();
    device_info.app_version = request->app_version();
    
    json_loader.insertJsonToDatabase(device_info);

    response->set_client_response(SERVICE_EXECUTED_SUCCESFULLY);

    return grpc::Status::OK;
}