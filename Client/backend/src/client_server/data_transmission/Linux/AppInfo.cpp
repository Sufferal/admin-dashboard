#include "../AppInfo.h"

std::optional<std::string> AppInfo::getInstallerVersion() {
  std::string version;

  // Use popen to run the dpkg-query command and read its output
  FILE* pipe = popen("dpkg-query -W -f='${Version}' debian-installer", "r");
  if (pipe) {
      char buffer[128] = {};
      if (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
          size_t len = strlen(buffer);
          if (len > 0 && buffer[len - 1] == '\n') {
              buffer[len - 1] = '\0';
          }
          version = buffer;
      }
      pclose(pipe);
  }

  return version;
}