#pragma once

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <optional>


class AppInfo {
public:  
    static std::optional<std::string> getInstallerVersion();
};