#pragma once

#include <cstring>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <net/if.h>
#include <unistd.h>

class IpAddress {
public:  
    std::string getLocalIPAddress();
    std::string getGatewayInterface();
};