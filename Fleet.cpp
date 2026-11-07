#include "Fleet.h"

#include <algorithm>

std::shared_ptr<ModelSpec> Fleet::getOrCreateModel(const std::string& name) {
  for (auto& spec : models_) {
    if (spec->modelName == name) return spec;
  }
  auto spec = std::make_shared<ModelSpec>(name);
  models_.add(spec);
  return spec;
}

const InventoryItem* Fleet::findById(const std::string& id) const {
  for (const auto& item : items_) {
    if (item->inventoryId() == id) return item.get();
  }
  return nullptr;
}

void Fleet::ensureUniqueId(const std::string& id) const {
  if (findById(id) != nullptr) {
    throw InvalidArgument("duplicate id: " + id);
  }
}

void Fleet::addScooter(const std::string& id, const std::string& name,
  double energy, int score) {
  if (id.empty()) throw InvalidArgument("id must not be empty");
  if (energy < 0) throw InvalidArgument("energy must be >= 0");
  if (score < 1 || score > 10) throw InvalidArgument("score must be in 1..10");
  service_.inspect(name);
  ensureUniqueId(id);
  auto spec = getOrCreateModel(name);
  items_.push_back(std::make_unique<ElectricScooter>(
    id, name, std::move(spec), score, energy));
}

void Fleet::addBike(const std::string& id, const std::string& name, int score) {
  if (id.empty()) throw InvalidArgument("id must not be empty");
  if (score < 1 || score > 10) throw InvalidArgument("score must be in 1..10");
  service_.inspect(name);
  ensureUniqueId(id);
  auto spec = getOrCreateModel(name);
  items_.push_back(std::make_unique<Bike>(
    id, name, std::move(spec), score));
}

void Fleet::addEBike(const std::string& id, const std::string& name,
  double energy, int score) {
  if (id.empty()) throw InvalidArgument("id must not be empty");
  if (energy < 0) throw InvalidArgument("energy must be >= 0");
  if (score < 1 || score > 10) throw InvalidArgument("score must be in 1..10");
  service_.inspect(name);
  ensureUniqueId(id);
  auto spec = getOrCreateModel(name);
  items_.push_back(std::make_unique<EBike>(
    id, name, std::move(spec), score, energy));
}

void Fleet::addHelmet(const std::string& id, const std::string& name) {
  if (id.empty()) throw InvalidArgument("id must not be empty");
  ensureUniqueId(id);
  items_.push_back(std::make_unique<Helmet>(id, name));
}

void Fleet::addDock(const std::string& id, const std::string& name, double energy) {
  if (id.empty()) throw InvalidArgument("id must not be empty");
  if (energy < 0) throw InvalidArgument("energy must be >= 0");
  ensureUniqueId(id);
  items_.push_back(std::make_unique<Dock>(id, name, energy));
}

double Fleet::totalEnergy() const {
  double sum = 0.0;
  for (const auto& item : items_) {
    if (const auto* ec = dynamic_cast<const EnergyConsumer*>(item.get())) {
      sum += ec->dailyEnergy();
    }
  }
  return sum;
}

void Fleet::printNovice(std::ostream& os) const {
  std::vector<const Transport*> novices;
  for (const auto& item : items_) {
    if (const auto* t = dynamic_cast<const Transport*>(item.get())) {
      if (t->noviceFriendly()) novices.push_back(t);
    }
  }
  std::sort(novices.begin(), novices.end(),
    [](const InventoryItem* a, const InventoryItem* b) {
      return *a < *b;
    });
  for (const auto* t : novices) os << *t << '\n';
}

void Fleet::printList(std::ostream& os) const {
  std::vector<const InventoryItem*> sorted;
  sorted.reserve(items_.size());
  for (const auto& item : items_) sorted.push_back(item.get());
  std::sort(sorted.begin(), sorted.end(),
    [](const InventoryItem* a, const InventoryItem* b) {
      return *a < *b;
    });
  for (const auto* item : sorted) os << *item << '\n';
}

void Fleet::focus(const std::string& id) {
  const auto* item = findById(id);
  if (item == nullptr) throw InvalidArgument("no such id: " + id);
  focusOwner_ = std::make_shared<FocusInfo>(id, item->name());
  focus_ = focusOwner_;
  focusState_ = FocusState::Active;
}

void Fleet::showFocus(std::ostream& os) const {
  if (focusState_ == FocusState::Active) {
    if (auto p = focus_.lock()) {
      os << p->id << ' ' << p->name;
      return;
    }
  }
  if (focusState_ == FocusState::Removed) os << "removed";
  else os << "expired";
}

void Fleet::remove(const std::string& id) {
  auto it = std::find_if(items_.begin(), items_.end(),
    [&](const auto& p) { return p->inventoryId() == id; });
  if (it == items_.end()) throw InvalidArgument("no such id: " + id);
  if (auto p = focus_.lock(); p && p->id == id) {
    focusOwner_.reset();
    focusState_ = FocusState::Removed;
  }
  items_.erase(it);
}