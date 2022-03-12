#include "Spg.h"

Spg::Spg(int x, int y, int z, int spawn_x, int spawn_y, int spawn_z,
         int health_points, int capture_points, int id)
        : Tank(x, y, z, spawn_x, spawn_y, spawn_z, health_points, capture_points, id) {
    this->type_ = TankType::SPG;
    this->speed_points_ = 1;
    this->damage_ = 1;
    this->max_shot_radius_ = 3;
    this->min_shot_radius_ = 3;
    this->destruction_points_ = 1;
    this->max_health_points_ = 1;
}