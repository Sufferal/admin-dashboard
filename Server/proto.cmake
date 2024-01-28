find_package(protobuf REQUIRED)
find_package(gRPC REQUIRED)

set(PROTO_INCLUDE_DIRS ${PROJECT_ROOT_DIR}/../Shared/proto)
set(GENERATED_PROTO_DIR ${PROTO_INCLUDE_DIRS}/build)
set(PROTO_FILES ${PROJECT_ROOT_DIR}/../Shared/proto/client_data_transmission.proto 
                ${PROJECT_ROOT_DIR}/../Shared/proto/service_call_result.proto 
                ${PROJECT_ROOT_DIR}/../Shared/proto/server_instructions.proto
                ${PROJECT_ROOT_DIR}/../Shared/proto/manual_update_service.proto
                ${PROJECT_ROOT_DIR}/../Shared/proto/os_type.proto
                ) 

add_library(proto ${PROTO_FILES})

target_link_libraries(proto
        protobuf::libprotobuf
        gRPC::grpc
        gRPC::grpc++
)

target_include_directories(proto PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

set(PROTO_BINARY_DIR ${GENERATED_PROTO_DIR})

protobuf_generate(
    TARGET proto
    IMPORT_DIRS ${PROTO_INCLUDE_DIRS}
    PROTOC_OUT_DIR "${PROTO_BINARY_DIR}")

protobuf_generate(
    TARGET proto
    LANGUAGE grpc
    GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc
    PLUGIN "protoc-gen-grpc=$<TARGET_FILE:gRPC::grpc_cpp_plugin>"
    IMPORT_DIRS ${PROTO_INCLUDE_DIRS}
    PROTOC_OUT_DIR "${PROTO_BINARY_DIR}")