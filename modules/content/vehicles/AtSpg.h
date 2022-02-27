#pragma once

#include "Tank.h"
#include "../../map/Hex.h"
#include "../Content.h"

class AtSpg : public Tank {
public:
    AtSpg(int x, int y, int z, int spawn_x, int spawn_y, int spawn_z,
          int health_points, int capture_points, int id);

    virtual std::vector<PosList> getShootingHexesAreas(Map &map) const override;
};