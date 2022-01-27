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

    void setPlayerId(int player_id);

    Hex getPosition() const;

    friend std::ostream &operator<<(std::ostream &stream, const Tank &tank);

protected:
    TankType type_;
    ContentType content_type;
    int x_;
    int y_;
    int z_;
    int health_points_;
    int capture_points_;
    int speed_points_;
    int damage_;
    int destruction_points_;
    int player_id_ = -1;
    Strategy current_strategy_;
};

