// ESETServerSide.cpp : Defines the entry point for the application.
#include "ESETServerSide.h"


int main(int argc, char** argv) {

    ServerHttp app;
    DatabaseHandler& db = DatabaseSingleton::getInstance();
    ClientDataProcessingImpl admin_service_instance;
    IpAddress local_ip;

    int PORT = 7070;
    const int PORTRPC = 2048;

    std::string ip_address = local_ip.getLocalIPAddress();
    if (!ip_address.empty()) {
        std::cout << "Local IP address: " << ip_address << std::endl;
    } else {
        std::cout << "Unable to detect local IP address." << std::endl;
    }

    std::string server_address = ip_address + ":" + std::to_string(PORT);
    
	grpc::ServerBuilder gui_builder;
	gui_builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
	gui_builder.RegisterService(&admin_service_instance);

	std::unique_ptr<grpc::Server> gui_server(gui_builder.BuildAndStart());
	std::cout << "GUI server listening on " << server_address << std::endl;

    if (!db.connect()) {
        std::cerr << "Failed to connect to MongoDB" << std::endl;
    } else {
        std::cout << "Database connection was successful" << std::endl;
    }
    
    std::string server_address_rpc = ip_address + ":" + std::to_string(PORTRPC);

    ManualUpdateServiceImpl service;
    grpc::ServerBuilder builder;

    builder.AddListeningPort(server_address_rpc, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address_rpc << " for manual update."<<std::endl;

    return app.run(argc, argv);
}
