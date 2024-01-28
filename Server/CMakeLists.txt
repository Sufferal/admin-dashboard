#  CMakeList.txt : CMake project for ESETServerSide, include source and define
# project specific logic here.

cmake_minimum_required (VERSION 3.8)

project(ESETServerSide)

get_filename_component(PROJECT_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}" ABSOLUTE)
cmake_minimum_required (VERSION 3.8)
include(proto.cmake)

# Find and link POCO libraries
find_package(Poco REQUIRED Net Util Foundation)
include_directories(${Poco_INCLUDE_DIRS})
link_directories(${Poco_LIBRARY_DIRS})

# Find the mongocxx driver
find_package(mongocxx REQUIRED)
find_package(bsoncxx REQUIRED)

# Find and link OpenSSL
find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

# Find nlohmann_json
find_package(nlohmann_json 3.2.0 REQUIRED)

# Set the source files
set(SOURCES
        ESETServerSide.cpp
        ${PROJECT_ROOT_DIR}/src/include/AdminClient.h
        ${PROJECT_ROOT_DIR}/src/entity/AdminClient.cpp 
        ${PROJECT_ROOT_DIR}/src/entity/ServerHttp.cpp
        ${PROJECT_ROOT_DIR}/src/entity/DeviceHandler.cpp
        ${PROJECT_ROOT_DIR}/src/entity/UpdateHandler.cpp
        ${PROJECT_ROOT_DIR}/src/entity/ShutdownHandler.cpp
        ${PROJECT_ROOT_DIR}/src/entity/RestartHandler.cpp
        ${PROJECT_ROOT_DIR}/src/entity/LoginRequestBody.cpp
        ${PROJECT_ROOT_DIR}/src/entity/RequestHandlerFactory.cpp
        ${PROJECT_ROOT_DIR}/src/entity/RequestHandler.cpp
        ${PROJECT_ROOT_DIR}/src/entity/DatabaseHandler.cpp
        ${PROJECT_ROOT_DIR}/src/entity/JsonLoader.cpp
        ${PROJECT_ROOT_DIR}/src/entity/LoginDb.cpp
        ${PROJECT_ROOT_DIR}/src/entity/DatabaseControls.cpp
        ${PROJECT_ROOT_DIR}/src/entity/DatabaseSingleton.cpp
        ${PROJECT_ROOT_DIR}/src/entity/MongoHandler.cpp
        ${PROJECT_ROOT_DIR}/src/include/AdminClient.h
        ${PROJECT_ROOT_DIR}/src/entity/ClientDataProcessingImpl.cpp
        ${PROJECT_ROOT_DIR}/src/entity/IpAddress.cpp
        ${PROJECT_ROOT_DIR}/src/entity/InstallerServiceImpl.cpp
        ${PROJECT_ROOT_DIR}/src/include/ServerHttp.h
        ${PROJECT_ROOT_DIR}/src/include/DeviceHandler.h
        ${PROJECT_ROOT_DIR}/src/include/UpdateHandler.h
        ${PROJECT_ROOT_DIR}/src/include/RestartHandler.h
        ${PROJECT_ROOT_DIR}/src/include/ShutdownHandler.h
        ${PROJECT_ROOT_DIR}/src/include/LoginRequestBody.h
        ${PROJECT_ROOT_DIR}/src/include/RequestHandlerFactory.h
        ${PROJECT_ROOT_DIR}/src/include/RequestHandler.h
        ${PROJECT_ROOT_DIR}/src/include/DatabaseHandler.h
        ${PROJECT_ROOT_DIR}/src/include/JsonLoader.h
        ${PROJECT_ROOT_DIR}/src/include/LoginDb.h
        ${PROJECT_ROOT_DIR}/src/include/DatabaseControls.h
        ${PROJECT_ROOT_DIR}/src/include/DatabaseSingleton.h
        ${PROJECT_ROOT_DIR}/src/include/MongoHandler.h
        ${PROJECT_ROOT_DIR}/src/include/ClientDataProcessingImpl.h
        ${PROJECT_ROOT_DIR}/src/include/IpAddress.h
        ${PROJECT_ROOT_DIR}/src/include/InstallerServiceImpl.h
)

# Add your source files
add_executable(ESETServerSide ${SOURCES})

# Link against POCO libraries
target_link_libraries(ESETServerSide Poco::Net Poco::Util Poco::Foundation)
target_link_libraries(ESETServerSide mongo::mongocxx_shared mongo::bsoncxx_shared)
target_link_libraries(ESETServerSide ${OPENSSL_LIBRARIES})
target_link_libraries(ESETServerSide nlohmann_json::nlohmann_json)
target_link_libraries(ESETServerSide proto)

# Include proto library
target_include_directories(ESETServerSide PRIVATE "../Shared/proto/build")

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})