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

using PosList = std::vector<Position>;

class Tank : public Content {
public:
    Tank(int x, int y, int z, int spawn_x, int spawn_y, int spawn_z, int health_points, int capture_points, int id);

    Tank(json data, int id);

    virtual PosList getAchievableHexes(Map &map) const;

    virtual std::vector<PosList> getShootingHexesAreas(Map &map) const;

    void setPlayerId(int player_id);

    int getPlayerId();

    Position getPosition() const;

    int getSpeedPoints();

    TankType getTankType();

    std::string getStringTankType();

    int getHealthPoints();

    int getMaxHealthPoints();

    int getDamage();

    int getDestructionPoints();

    int getCapturePoints();

    Position getSpawnPosition();

    void update(int x, int y, int z, int health, int capture_points, int shoot_range_bonus);

    void update(int x, int y, int z);

    void update(int health);


    void update(Position);


    friend std::ostream &operator<<(std::ostream &stream, const Tank &tank);

    std::shared_ptr<StateMachine> current_strategy_;

    std::vector<Position> list_moves_ = {};
protected:

    TankType type_;
    ContentType content_type;
    Position pos_;
    Position spawn_pos_;
    int health_points_;
    int max_health_points_;
    int capture_points_;
    int speed_points_;
    int damage_;
    int max_shot_radius_ = 0;
    int min_shot_radius_ = 0;
    int shoot_range_bonus_ = 0;
    int destruction_points_;
    int player_id_ = -1;
};

