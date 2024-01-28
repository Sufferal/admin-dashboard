#include "../AdminInstructionsImpl.h"

#include <stdlib.h>
#include <fstream>
#include <Poco/Process.h>
#include <Poco/Exception.h>


grpc::Status AdminInstructionsImpl::Shutdown(grpc::ServerContext* context, const ShutdownRequest* request, ShutdownResponse* response) {
	response->set_shutdown_service_result(SERVICE_EXECUTED_SUCCESFULLY);
	system("c:\\windows\\system32\\shutdown /s");

	return grpc::Status::OK;
}

grpc::Status AdminInstructionsImpl::Restart(grpc::ServerContext* context, const RestartRequest* request, RestartResponse* response) {
	response->set_restart_service_result(SERVICE_EXECUTED_SUCCESFULLY);
	system("c:\\windows\\system32\\shutdown /r");

	return grpc::Status::OK;
}

grpc::Status AdminInstructionsImpl::UpdateApplication(grpc::ServerContext* context, grpc::ServerReader<UpdateApplicationRequest>* reader, UpdateApplicationResponse* response) {
	std::ofstream outfile(UpdateConstants::WINDOWS_UPDATE_FILE_NAME, std::ofstream::binary);
	
	UpdateApplicationRequest installer_chunk;
	while (reader->Read(&installer_chunk))
	{
		outfile.write(installer_chunk.app_installer().data(), installer_chunk.app_installer().size());
	}

	try
	{
		Poco::Process::launch("msiexec", { "/i", UpdateConstants::WINDOWS_UPDATE_FILE_NAME });
		response->set_update_application_service_result(SERVICE_EXECUTED_SUCCESFULLY);
	}
	catch (const Poco::Exception& ex)
	{
		response->set_update_application_service_result(SERVICE_FAILED_TO_EXECUTE);
		std::cerr << "Error: " << ex.displayText() << std::endl;
	}

	return grpc::Status::OK;
}