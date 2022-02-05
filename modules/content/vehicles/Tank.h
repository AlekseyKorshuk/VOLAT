#pragma once

#include <vector>

#include "../Content.h"
#include "../../map/Hex.h"
#include "../../map/Map.h"
#include "../../state_machine/StateMachine.h"

class StateMachine;

enum TankType : int {
    SPG = 0,
    LIGHT = 1,
    HEAVY = 2,
    MEDIUM = 3,
    AT_SPG = 4
};

using HexPtrList = std::vector<std::shared_ptr<Hex>>;

class Tank : public Content {
public:
    Tank(int x, int y, int z, int spawn_x, int spawn_y, int spawn_z, int health_points, int capture_points, int id);

    Tank(json data, int id);

    virtual HexPtrList getAchievableHexes(Map &map) const;

    virtual std::vector<HexPtrList> getShootingHexesAreas(Map &map) const;

    void setPlayerId(int player_id);

    int getPlayerId();

    Hex getPosition() const;

    int getSpeedPoints();

    TankType getTankType();

    int getHealthPoints();

    int getDamage();

    int getDestructionPoints();

    void update(int x, int y, int z, int health, int capture_points);

    void update(int x, int y, int z);

    void update(int health);

    void update(std::shared_ptr<Hex>);

    void update(Hex);


    friend std::ostream &operator<<(std::ostream &stream, const Tank &tank);

    std::shared_ptr<StateMachine> current_strategy_;


protected:
    HexPtrList getHexesInShotRadius(Map &map) const;

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

