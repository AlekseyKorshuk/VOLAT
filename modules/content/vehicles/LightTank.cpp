#include "LightTank.h"

LightTank::LightTank(int x, int y, int z, int spawn_x, int spawn_y, int spawn_z,
                     int health_points, int capture_points, int id)
    : Tank(x, y, z, spawn_x, spawn_y, spawn_z, health_points, capture_points, id) {
    this->type_ = TankType::LIGHT;
    this->speed_points_ = 3;
    this->damage_ = 1;
    this->shot_radius_ = 2;
    this->destruction_points_ = 1;
}