#ifndef TRANSPORT_H
#define TRANSPORT_H

#include "Inventory.h"
#include "Errors.h"
#include <memory>
#include <string>

struct ModelSpec {
  std::string modelName;
  explicit ModelSpec(std::string name) : modelName(std::move(name)) {}
};

class Transport : public InventoryItem, public EnergyConsumer {
public:
  Transport(std::string id, std::string name,
    std::shared_ptr<ModelSpec> spec, int simplicity)
    : id_(std::move(id)),
    name_(std::move(name)),
    spec_(std::move(spec)),
    simplicity_(simplicity) {
    if (simplicity_ < 1 || simplicity_ > 10) {
      throw InvalidArgument("simplicity must be in 1..10");
    }
  }

  const std::string& inventoryId() const override { return id_; }
  const std::string& name() const override { return name_; }

  bool noviceFriendly() const { return simplicity_ >= 6; }
  int simplicity() const { return simplicity_; }

  double dailyEnergy() const override { return 0.0; }

  const std::shared_ptr<ModelSpec>& spec() const { return spec_; }

protected:
  std::string id_;
  std::string name_;
  std::shared_ptr<ModelSpec> spec_;
  int simplicity_;
};

class Bike : public Transport {
public:
  Bike(std::string id, std::string name,
    std::shared_ptr<ModelSpec> spec, int simplicity)
    : Transport(std::move(id), std::move(name), std::move(spec), simplicity) {
  }

  std::string describe() const override {
    return "bike " + id_ + " " + name_ +
      " score " + std::to_string(simplicity_);
  }

  void print(std::ostream& os) const override {
    os << id_ << ' ' << name_;
  }
};

class ElectricScooter : public Transport {
public:
  ElectricScooter(std::string id, std::string name,
    std::shared_ptr<ModelSpec> spec, int simplicity,
    double energy)
    : Transport(std::move(id), std::move(name), std::move(spec), simplicity),
    energy_(energy) {
    if (energy_ < 0.0) {
      throw InvalidArgument("energy must be >= 0");
    }
  }

  double dailyEnergy() const override { return energy_; }

  std::string describe() const override {
    return "scooter " + id_ + " " + name_ +
      " energy " + std::to_string(energy_) +
      " score " + std::to_string(simplicity_);
  }

  void print(std::ostream& os) const override {
    os << id_ << ' ' << name_;
  }

private:
  double energy_;
};

class EBike : public Transport {
public:
  EBike(std::string id, std::string name,
    std::shared_ptr<ModelSpec> spec, int simplicity,
    double energy)
    : Transport(std::move(id), std::move(name), std::move(spec), simplicity),
    energy_(energy) {
    if (energy_ < 0.0) {
      throw InvalidArgument("energy must be >= 0");
    }
  }

  double dailyEnergy() const override { return energy_; }

  std::string describe() const override {
    return "ebike " + id_ + " " + name_ +
      " energy " + std::to_string(energy_) +
      " score " + std::to_string(simplicity_);
  }

  void print(std::ostream& os) const override {
    os << id_ << ' ' << name_;
  }

private:
  double energy_;
};

#endif
