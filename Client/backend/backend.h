// backend.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <string>
#include <optional>
#include <cstdlib>
#include <cstring>

// TODO: Reference additional headers your program requires here.
#include <grpc/grpc.h>
#include <grpcpp/server_builder.h>
#include <Poco/Util/ServerApplication.h>
#include <Poco/Util/IniFileConfiguration.h>

#include "gui_communication.grpc.pb.h"
#include "server_instructions.grpc.pb.h"
#include "client_data_transmission.grpc.pb.h"
#include "src/client_server/update_service/UpdateServiceImpl.h"
#include "src/client_server/admin_instructions/AdminInstructionsImpl.h"

#include "gui_communication.grpc.pb.h"
#include "server_instructions.grpc.pb.h"
