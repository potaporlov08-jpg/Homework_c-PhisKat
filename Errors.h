#ifndef ERRORS_HPP
#define ERRORS_HPP

#include <stdexcept>
#include <string>


class FleetError : public std::runtime_error {
public:
  explicit FleetError(const std::string& message)
    : std::runtime_error(message) {}
};


class InspectionRejected : public FleetError {
public:
  explicit InspectionRejected(const std::string& message = "inspection rejected")
    : FleetError(message) {
  }
};


class InvalidArgument : public FleetError {
public:
  explicit InvalidArgument(const std::string& message)
    : FleetError(message) {
  }
};

#endif