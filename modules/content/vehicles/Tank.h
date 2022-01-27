#pragma once

#include <vector>

#include "../Content.h"
#include "../../map/Hex.h"
#include "../../strategy/Strategy.h"

enum class TankType { // new ones will be added next time
    MEDIUM
};

using HexList = std::vector<Hex>;

class Tank : public Content {
public:
    Tank(int x, int y, int z, int health_points, int capture_points);
    Tank(json data);

    virtual HexList getAvailableHexesForMove() const = 0;

    virtual std::vector<HexList> getShootingHexesAreas() const = 0;

    Hex getPosition() const;

    TankType type_;
    ContentType content_type;
    int x_;
    int y_;
    int z_;
    int health_points;
    int capture_points;
    int speed_points;
    int damage;
    int destruction_points;

    Strategy current_strategy;

    friend std::ostream& operator<< (std::ostream& stream, const Tank& tank){
        stream << tank.getPosition() << " HP: " << tank.health_points << ", CP: " << tank.capture_points << ", SP: " << tank.speed_points;
        return stream;
    }



};

