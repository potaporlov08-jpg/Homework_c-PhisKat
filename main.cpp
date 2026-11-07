#include "Fleet.h"
#include "Errors.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace {
  void requireEnd(std::istringstream& in, const std::string& cmd) {
    std::string extra;
    if (in >> extra) {
      throw InvalidArgument("unexpected argument in '" + cmd + "': " + extra);
    }
  }

  void cmdAdd(Fleet& fleet, std::istringstream& in) {
    std::string kind;
    if (!(in >> kind)) throw InvalidArgument("add: missing kind");

    if (kind == "scooter") {
      std::string id, name; double energy; int score;
      if (!(in >> id >> name >> energy >> score))
        throw InvalidArgument("add scooter: expected id name energy score");
      requireEnd(in, "add scooter");
      fleet.addScooter(id, name, energy, score);
    }
    else if (kind == "bike") {
      std::string id, name; int score;
      if (!(in >> id >> name >> score))
        throw InvalidArgument("add bike: expected id name score");
      requireEnd(in, "add bike");
      fleet.addBike(id, name, score);
    }
    else if (kind == "ebike") {
      std::string id, name; double energy; int score;
      if (!(in >> id >> name >> energy >> score))
        throw InvalidArgument("add ebike: expected id name energy score");
      requireEnd(in, "add ebike");
      fleet.addEBike(id, name, energy, score);
    }
    else if (kind == "helmet") {
      std::string id, name;
      if (!(in >> id >> name))
        throw InvalidArgument("add helmet: expected id name");
      requireEnd(in, "add helmet");
      fleet.addHelmet(id, name);
    }
    else if (kind == "dock") {
      std::string id, name; double energy;
      if (!(in >> id >> name >> energy))
        throw InvalidArgument("add dock: expected id name energy");
      requireEnd(in, "add dock");
      fleet.addDock(id, name, energy);
    }
    else {
      throw InvalidArgument("unknown add kind: " + kind);
    }
  }

  bool processLine(Fleet& fleet, const std::string& line) {
    std::istringstream in(line);
    std::string cmd;
    if (!(in >> cmd)) return true;

    if (cmd == "add") {
      cmdAdd(fleet, in);
    }
    else if (cmd == "energy") {
      requireEnd(in, "energy");
      auto flags = std::cout.flags();
      std::cout << std::fixed << std::setprecision(1)
        << fleet.totalEnergy() << '\n';
      std::cout.flags(flags);
    }
    else if (cmd == "novice") {
      requireEnd(in, "novice");
      fleet.printNovice(std::cout);
    }
    else if (cmd == "list") {
      requireEnd(in, "list");
      fleet.printList(std::cout);
    }
    else if (cmd == "focus") {
      std::string id;
      if (!(in >> id)) throw InvalidArgument("focus: missing id");
      requireEnd(in, "focus");
      fleet.focus(id);
    }
    else if (cmd == "show_focus") {
      requireEnd(in, "show_focus");
      fleet.showFocus(std::cout);
      std::cout << '\n';
    }
    else if (cmd == "remove") {
      std::string id;
      if (!(in >> id)) throw InvalidArgument("remove: missing id");
      requireEnd(in, "remove");
      fleet.remove(id);
    }
    else if (cmd == "quit") {
      requireEnd(in, "quit");
      return false;
    }
    else {
      throw InvalidArgument("unknown command: " + cmd);
    }
    return true;
  }

}

int main() {
  Fleet fleet;
  std::string line;
  while (std::getline(std::cin, line)) {
    try {
      if (!processLine(fleet, line)) break;
    }
    catch (const FleetError& e) {
      std::cout << "error: " << e.what() << '\n';
    }
    catch (const std::exception& e) {
      std::cout << "error: " << e.what() << '\n';
    }
  }
  return 0;
}