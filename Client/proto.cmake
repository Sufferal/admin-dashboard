find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

set(PROTO_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../Shared/proto)
set(PROTO_FILES ${PROTO_INCLUDE_DIRS}/gui_communication.proto 
                ${PROTO_INCLUDE_DIRS}/server_instructions.proto
                ${PROTO_INCLUDE_DIRS}/service_call_result.proto
                ${PROTO_INCLUDE_DIRS}/client_data_transmission.proto
                ${PROTO_INCLUDE_DIRS}/manual_update_service.proto
                ${PROTO_INCLUDE_DIRS}/os_type.proto)

set(GENERATED_PROTO_DIR ${PROTO_INCLUDE_DIRS}/build)

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