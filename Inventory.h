#ifndef INVENTORY_H
#define INVENTORY_H

#include <ostream>
#include <string>

class InventoryItem {
public:
  virtual ~InventoryItem() = default;

  virtual const std::string& inventoryId() const = 0;
  virtual const std::string& name() const = 0;
  virtual std::string describe() const = 0;

  virtual void print(std::ostream& os) const = 0;
};

class EnergyConsumer {
public:
  virtual ~EnergyConsumer() = default;
  virtual double dailyEnergy() const = 0;
};

inline bool operator<(const InventoryItem& lhs, const InventoryItem& rhs) {
  return lhs.inventoryId() < rhs.inventoryId();
}

inline std::ostream& operator<<(std::ostream& os, const InventoryItem& item) {
  item.print(os);
  return os;
}

#endif
