#ifndef SERVICECENTER_H
#define SERVICECENTER_H

#include "Errors.h"
#include "Inventory.h"
#include <algorithm>
#include <cctype>
#include <string>

class ServiceCenter {
public:
  void inspect(const std::string& name) const {
    std::string lower = name;
    std::transform(lower.begin(), lower.end(), lower.begin(),
      [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

    if (lower.find("broken") != std::string::npos) {
      throw InspectionRejected("inspection rejected");
    }
  }
};

#endif
