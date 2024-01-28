# CMakeList.txt : CMake project for backend, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("backend")

set(CMAKE_BUILD_TYPE Release)

find_package(Poco REQUIRED Foundation Util Net)

# Add source to this project's executable.
add_executable (backend "backend.cpp" "backend.h"
			"src/client_server/update_service/UpdateServiceImpl.h"
			"src/client_server/update_service/UpdateServiceImpl.cpp"
			"src/client_server/admin_instructions/AdminInstructionsImpl.h"
			"src/client_server/data_transmission/DataInserterClientImpl.h"
			"src/client_server/data_transmission/DataInserterClientImpl.cpp"
			"src/client_server/data_transmission/DeviceInfo.h"
      		"src/client_server/data_transmission/AppInfo.h"
			"src/client_client/manual_update/ManualUpdateClient.h"
			"src/include/UpdateConstants.h")

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
	target_link_libraries(backend PRIVATE wsock32 ws2_32)
    target_sources(backend PRIVATE
				"src/client_server/admin_instructions/Windows/AdminInstructionsImpl.cpp"
				"src/client_server/data_transmission/Windows/DeviceInfo.cpp"
				"src/client_server/data_transmission/Windows/AppInfo.cpp"
				"src/client_client/manual_update/Windows/ManualUpdateClient.cpp")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_sources(backend PRIVATE
				"src/client_server/admin_instructions/Linux/AdminInstructionsImpl.cpp"
				"src/client_server/data_transmission/Linux/DeviceInfo.cpp"
        		"src/client_server/data_transmission/Linux/AppInfo.cpp"
				"src/client_client/manual_update/Linux/ManualUpdateClient.cpp")
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET backend PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(backend PRIVATE proto)
target_link_libraries(backend PRIVATE Poco::Foundation Poco::Util Poco::Net)

target_include_directories(backend PRIVATE "../../Shared/proto/build")

# setup Wix installer tool
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

set(CPACK_PACKAGE_NAME ClientApp)
set(CPACK_PACKAGE_VERSION ${CMAKE_PROJECT_VERSION})
set(CPACK_GENERATOR "WIX")

set(CPACK_WIX_UPGRADE_GUID "910EC802-E05D-47AE-AAA9-3459844CAF44")
set(CPACK_WIX_TEMPLATE  "${CMAKE_CURRENT_SOURCE_DIR}/client_app_install.xml")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "ClientApplication")

include(CPack)