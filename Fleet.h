#ifndef FLEET_H
#define FLEET_H

#include "Errors.h"
#include "Inventory.h"
#include "Registry.h"
#include "ServiceCenter.h"
#include "Things.h"
#include "Transport.h"

#include <memory>
#include <ostream>
#include <string>
#include <vector>

struct FocusInfo {
  std::string id;
  std::string name;
  FocusInfo(std::string i, std::string n)
    : id(std::move(i)), name(std::move(n)) {
  }
};

class Fleet {
public:
  Fleet() = default;

  void addScooter(const std::string& id, const std::string& name,
    double energy, int score);
  void addBike(const std::string& id, const std::string& name, int score);
  void addEBike(const std::string& id, const std::string& name,
    double energy, int score);
  void addHelmet(const std::string& id, const std::string& name);
  void addDock(const std::string& id, const std::string& name, double energy);

  double totalEnergy() const;
  void printNovice(std::ostream& os) const;
  void printList(std::ostream& os) const;

  void focus(const std::string& id);
  void showFocus(std::ostream& os) const;
  void remove(const std::string& id);

private:
  enum class FocusState { None, Active, Removed };

  std::shared_ptr<ModelSpec> getOrCreateModel(const std::string& name);
  const InventoryItem* findById(const std::string& id) const;
  void ensureUniqueId(const std::string& id) const;

  std::vector<std::unique_ptr<InventoryItem>> items_;
  Registry<std::shared_ptr<ModelSpec>, 16> models_;
  ServiceCenter service_;

  std::shared_ptr<FocusInfo> focusOwner_;
  std::weak_ptr<FocusInfo> focus_;
  FocusState focusState_ = FocusState::None;
};

#endif
