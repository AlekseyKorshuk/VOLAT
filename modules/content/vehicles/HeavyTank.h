#pragma once

#include "Tank.h"
#include "../../map/Hex.h"
#include "../Content.h"

class HeavyTank : public Tank {
public:
    /**
     * Tank constructor
     * @param x Coordinate X
     * @param y Coordinate Y
     * @param z Coordinate Z
     * @param spawn_x Coordinate X of the spawn
     * @param spawn_y Coordinate Y of the spawn
     * @param spawn_z Coordinate Z of the spawn
     * @param health_points Amount of health points
     * @param capture_points Amount of capture points
     * @param id Object id
     */
    HeavyTank(int x, int y, int z, int spawn_x, int spawn_y, int spawn_z,
              int health_points, int capture_points, int id);
};