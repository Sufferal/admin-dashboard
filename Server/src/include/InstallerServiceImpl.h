#pragma once

#include <grpcpp/grpcpp.h>
#include "../../../Shared/proto/build/manual_update_service.grpc.pb.h"
#include <optional>

class ManualUpdateServiceImpl final : public ManualUpdateService::Service {
public:
    grpc::Status PerformManualUpdate(grpc::ServerContext* context, const ManualUpdateRequest* request, grpc::ServerWriter<ManualUpdateResponse>* writer) override;
    
private:
    std::optional<std::string> getLatestVersion(OsType osType);
};