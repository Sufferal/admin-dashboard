#include "backend.h"
#include <iostream>
#include "src/client_server/admin_instructions/AdminInstructionsImpl.h"
#include "src/client_server/data_transmission/DataInserterClientImpl.h"
#include "src/client_server/data_transmission/DeviceInfo.h"
#include "src/client_server/data_transmission/AppInfo.h"

class MyServerApp : public Poco::Util::ServerApplication {

private:
	const std::string GUI_SERVER_ADDRESS = "localhost:8080";
	const std::string ADMIN_INSTRUCTIONS_RECEIVER_ADDRESS = "0.0.0.0:9090";
	const std::string SERVER_ADDRESS = "intern.server:7070";
	
	int main(const std::vector<std::string>& args) override {

		DataInserterClientImpl client(grpc::CreateChannel(SERVER_ADDRESS, grpc::InsecureChannelCredentials()));

		ClientDataInserterRequest request;
		
		request.set_device_name(DeviceInfo::getDeviceName().value_or("default_device_name"));
		request.set_ip_address(DeviceInfo::getIpAddress().value_or("default_ip_address"));
		request.set_os_type(DeviceInfo::getOsType());
		request.set_app_version(AppInfo::getInstallerVersion().value_or("default_installer_version"));

		ClientDataInserterResponse response = client.SendClientData(request);

		UpdateServiceImpl update_service_instance;
		AdminInstructionsImpl admin_service_instance;

		grpc::ServerBuilder gui_builder;
		gui_builder.AddListeningPort(GUI_SERVER_ADDRESS, grpc::InsecureServerCredentials());
		gui_builder.RegisterService(&update_service_instance);

		std::unique_ptr<grpc::Server> gui_server(gui_builder.BuildAndStart());
		std::cout << "GUI server listening on " << GUI_SERVER_ADDRESS << std::endl;

		grpc::ServerBuilder admin_builder;
		admin_builder.AddListeningPort(ADMIN_INSTRUCTIONS_RECEIVER_ADDRESS, grpc::InsecureServerCredentials());
		admin_builder.RegisterService(&admin_service_instance);

		std::unique_ptr<grpc::Server> admin_server(admin_builder.BuildAndStart());
		std::cout << "Admin server listening on " << ADMIN_INSTRUCTIONS_RECEIVER_ADDRESS << std::endl;

		waitForTerminationRequest();

		return Application::EXIT_OK;
	}
};

int main(char argc, char** argv)
{
	MyServerApp app;
	return app.run(argc, argv);
}
