#include "MediumTank.h"

HexList MediumTank::getAvailableHexesForMove() const {
    return {};
}

std::vector<HexList> MediumTank::getShootingHexesAreas() const {
    return {};
}

MediumTank::MediumTank(int x, int y, int z, int health_points, int capture_points)
    : Tank(x, y, z, health_points, capture_points) {
    this->type_ = TankType::MEDIUM;
    this->speed_points_ = 2;
    this->damage_ = 1;
    this->destruction_points_ = 2;
}