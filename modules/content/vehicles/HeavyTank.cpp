#include "HeavyTank.h"

HeavyTank::HeavyTank(int x, int y, int z, int spawn_x, int spawn_y, int spawn_z,
                     int health_points, int capture_points, int id)
    : Tank(x, y, z, spawn_x, spawn_y, spawn_z, health_points, capture_points, id) {
    this->type_ = TankType::HEAVY;
    this->speed_points_ = 1;
    this->damage_ = 1;
    this->shot_radius_ = 2;
    this->destruction_points_ = 3;
}

std::vector<HexPtrList> HeavyTank::getShootingHexesAreas(Map& map) const {
    auto hexes_in_shot_radius = getHexesInShotRadius(map);
    std::vector<HexPtrList> areas;

    for (const auto& hex : hexes_in_shot_radius) {
        areas.push_back(HexPtrList(1, hex));
    }
    map.clearPath();

    return areas;
}