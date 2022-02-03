#pragma once

#include <vector>

#include "../Content.h"
#include "../../map/Hex.h"
#include "../../map/Map.h"
#include "../../state_machine/StateMachine.h"

class StateMachine;


enum class TankType { // new ones will be added next time
    MEDIUM
};

using HexList = std::vector<Hex>;

class Tank : public Content {
public:
    Tank(int x, int y, int z, int spawn_x, int spawn_y, int spawn_z, int health_points, int capture_points, int id);

    Tank(json data, int id);

        virtual HexList getAvailableHexesForMove(const Map& map) const;
        virtual std::vector<HexList> getShootingHexesAreas(const Map& map) const;

    void setPlayerId(int player_id);
    int getPlayerId();

    Hex getPosition() const;
    int getSpeedPoints();
    TankType getTankType();
    int getHealthPoints();

    void update(int x, int y, int z, int health, int capture_points);
    void update(int x, int y, int z);
    void update(int health);

    friend std::ostream &operator<<(std::ostream &stream, const Tank &tank);

    StateMachine* current_strategy_;



protected:
    TankType type_;
    ContentType content_type;
    int x_;
    int y_;
    int z_;
    int spawn_x_;
    int spawn_y_;
    int spawn_z_;
    int health_points_;
    int capture_points_;
    int speed_points_;
    int damage_;
    int shot_radius_;
    int destruction_points_;
    int player_id_ = -1;
};

