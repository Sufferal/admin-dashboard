#include "../DeviceInfo.h"

#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <iphlpapi.h>


std::optional<std::string> DeviceInfo::getDeviceName() {
    char computerName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = sizeof(computerName);

    if (GetComputerNameA(computerName, &size)) {
        return std::string(computerName);
    } else {
        return std::nullopt;
    }
}

OsType DeviceInfo::getOsType() {
    return OsType::WINDOWS;
}

std::optional<std::string> DeviceInfo::getIpAddress() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return std::nullopt;
    }

    ULONG outBufLen = 0;
    PIP_ADAPTER_ADDRESSES adapterAddresses = nullptr;

    DWORD result = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, nullptr, adapterAddresses, &outBufLen);
    if (result == ERROR_BUFFER_OVERFLOW) {
        adapterAddresses = reinterpret_cast<IP_ADAPTER_ADDRESSES*>(new char[outBufLen]);
        result = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, nullptr, adapterAddresses, &outBufLen);
    }

    if (result != ERROR_SUCCESS) {
        WSACleanup();
        delete[] reinterpret_cast<char*>(adapterAddresses);
        return std::nullopt;
    }

    std::string privateIpv4;

    for (PIP_ADAPTER_ADDRESSES adapter = adapterAddresses; adapter != nullptr; adapter = adapter->Next) {
        for (PIP_ADAPTER_UNICAST_ADDRESS unicastAddress = adapter->FirstUnicastAddress; unicastAddress != nullptr; unicastAddress = unicastAddress->Next) {
            sockaddr* sa = unicastAddress->Address.lpSockaddr;
            char ipAddrStr[INET_ADDRSTRLEN];
            int family = sa->sa_family;

            if (family == AF_INET) {
                sockaddr_in* sockaddrIPv4 = reinterpret_cast<sockaddr_in*>(sa);
                inet_ntop(AF_INET, &(sockaddrIPv4->sin_addr), ipAddrStr, INET_ADDRSTRLEN);

                unsigned long addr = ntohl(sockaddrIPv4->sin_addr.s_addr);
                if ((addr >> 24) == 10 ||
                    ((addr >> 20) & 0xFF) == 0xAC1 ||
                    ((addr >> 16) & 0xFF) == 0xC0A8) {
                    privateIpv4 = ipAddrStr;
                    break;
                }
            }
        }
        if (!privateIpv4.empty()) {
            break;
        }
    }

    delete[] reinterpret_cast<char*>(adapterAddresses);
    WSACleanup();

    if (!privateIpv4.empty()) {
        return privateIpv4;
    }
    else {
        return std::nullopt;
    }
}