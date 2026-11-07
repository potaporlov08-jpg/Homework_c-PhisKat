#ifndef THINGS_H
#define THINGS_H

#include "Inventory.h"
#include "Errors.h"
#include <memory>
#include <string>

class Helmet : public InventoryItem {
public:
  Helmet(std::string id, std::string name)
    : id_(std::move(id)), name_(std::move(name)) {
    if (id_.empty()) {
      throw InvalidArgument("id must not be empty");
    }
  }

  const std::string& inventoryId() const override { return id_; }
  const std::string& name() const override { return name_; }

  std::string describe() const override {
    return "helmet " + id_ + " " + name_;
  }

  void print(std::ostream& os) const override {
    os << id_ << ' ' << name_;
  }

private:
  std::string id_;
  std::string name_;
};

class Dock : public InventoryItem, public EnergyConsumer {
public:
  Dock(std::string id, std::string name, double energy)
    : id_(std::move(id)), name_(std::move(name)), energy_(energy) {
    if (id_.empty()) {
      throw InvalidArgument("id must not be empty");
    }
    if (energy_ < 0.0) {
      throw InvalidArgument("energy must be >= 0");
    }
  }

  const std::string& inventoryId() const override { return id_; }
  const std::string& name() const override { return name_; }
  double dailyEnergy() const override { return energy_; }

  std::string describe() const override {
    return "dock " + id_ + " " + name_ +
      " energy " + std::to_string(energy_);
  }

  void print(std::ostream& os) const override {
    os << id_ << ' ' << name_;
  }

private:
  std::string id_;
  std::string name_;
  double energy_;
};

#endif
