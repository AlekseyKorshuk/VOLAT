#pragma once

#include "Tank.h"


class MediumTank : public Tank {
public:
    MediumTank(int x, int y, int z, int health_points, int capture_points);

    virtual HexList getAvailableHexesForMove() const;

    virtual std::vector<HexList> getShootingHexesAreas() const;
};