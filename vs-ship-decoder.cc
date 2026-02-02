#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

#include "lnbt.hpp"
#include "zstr.hpp"
#include "nlohmann/json.hpp"

struct Ship {
  int id;
  int x, y, z;
  float mass;
  std::string name;
};

static inline std::ostream& operator<<(std::ostream &os, const Ship& ship) {
  os << std::setw(6) << std::right << ship.id;
  os << std::setw(10) << std::right << ship.x;
  os << std::setw(10) << std::right << ship.y;
  os << std::setw(10) << std::right << ship.z;
  os << std::setw(10) << std::right << static_cast<int64_t>(ship.mass);
  os << "   " << std::left << ship.name;
  return os;
}

int main(int argc, char *argv[]) {
  try {
    auto in_file = zstr::ifstream("vs_ship_data.dat", std::ios::binary);

    nbt::NBT data = nbt::bin::read(in_file);
    const nbt::Tag *a = data.tag.get_if("data");
    assert(a);

    const auto *b = a->get_if<std::vector<std::int8_t>>("vs_pipeline");
    assert(b);

    const std::string str(reinterpret_cast<const char*>(b->data()), b->size());
    // std::cout << str << "\n";

    std::vector<Ship> ships;
    auto json = nlohmann::json::parse(str);
    for (const auto &ship: json["ships"]) {
      Ship s;

      s.id = ship["id"].get<int>();
      s.name = ship["name"].get<std::string>();

      const auto pos = ship["transform"]["shipPositionInWorldCoordinates"];
      s.x = pos["x"].get<float>();
      s.y = pos["y"].get<float>();
      s.z = pos["z"].get<float>();

      s.mass = ship["inertiaData"]["shipMass"].get<float>();

      ships.push_back(std::move(s));
    }

    std::ranges::sort(ships, {}, &Ship::id);

    for (const auto &ship : ships) {
      std::cout << ship << "\n";
    }
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  return 0;
}
