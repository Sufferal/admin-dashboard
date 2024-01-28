#include "../DeviceInfo.h"

#include <fstream>
#include <net/if.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/utsname.h>


std::optional<std::string> DeviceInfo::getDeviceName() {
    std::ifstream hostnameFile("/proc/sys/kernel/hostname");
    if (hostnameFile) {
        std::string deviceName;
        std::getline(hostnameFile, deviceName);
        return deviceName;
    } else {
        return std::nullopt;
    }
}

OsType DeviceInfo::getOsType() {
    return OsType::LINUX;
}



std::optional<std::string> DeviceInfo::getGatewayInterface() {
    FILE* fp = popen("ip route | grep default | awk '{print $5}'", "r");
    if (!fp) {
        return std::nullopt;
    }

    char buffer[128] = {};
    std::string interface;
    if (fgets(buffer, sizeof(buffer), fp) != nullptr) {
        interface = buffer;
        interface.pop_back();
    }

    pclose(fp);
    return interface;
}

std::optional<std::string> DeviceInfo::getIpAddress() {
    std::optional<std::string> target_interface = getGatewayInterface();

    if (target_interface == std::nullopt) {
        std::cerr << "Failed to determine the default interface." << std::endl;
        return std::nullopt;
    }

    struct ifaddrs* ifaddr = nullptr;
    if (getifaddrs(&ifaddr) == -1) {
        return std::nullopt;
    }

    for (struct ifaddrs* ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == nullptr) {
            continue;
        }

        if (ifa->ifa_addr->sa_family == AF_INET) {
            std::string interface_name(ifa->ifa_name);
            if (interface_name == target_interface) {
                struct sockaddr_in* sockaddr_ipv4 = reinterpret_cast<struct sockaddr_in*>(ifa->ifa_addr);
                char ip_address[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &(sockaddr_ipv4->sin_addr), ip_address, INET_ADDRSTRLEN);
                freeifaddrs(ifaddr);
                return ip_address;
            }
        }
    }

    freeifaddrs(ifaddr);
    return std::nullopt;
}