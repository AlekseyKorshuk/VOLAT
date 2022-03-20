#pragma once

#include <vector>

#include "../Content.h"
#include "../../map/Hex.h"
//#include "../../map/Map.h"
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
    Tank(int x, int y, int z, int spawn_x, int spawn_y, int spawn_z, int health_points, int capture_points, int id);

    /**
     * Tank constructor from json
     * @param data Json data
     * @param id Tank ID
     */
    Tank(json data, int id);

    /**
     * Returns achievable hexes
     * @param map Game map
     * @return Resulting list
     */
    virtual PosList getAchievableHexes(Map &map) const;

    /**
     * Returns shooting hexes
     * @param map Game map
     * @return Resulting list
     */
    virtual std::vector<PosList> getShootingHexesAreas(Map &map) const;

    /**
     * Method to set player id
     * @param player_id ID
     */
    void setPlayerId(int player_id);

    /**
     * Method to get player id
     * @return Player ID
     */
    int getPlayerId();

    /**
     * Method to get current position of the tank
     * @return Position
     */
    Position getPosition() const;

    /**
     * Method to get speed points
     * @return Number of speed points
     */
    int getSpeedPoints();

    /**
     * Method to get tank type
     * @return Tank type
     */
    TankType getTankType();

    /**
     * method to get tank type as a string
     * @return String tank type
     */
    std::string getStringTankType();

    /**
     * Method to get tank health points
     * @return Number of health points
     */
    int getHealthPoints();

    /**
     * Method to get maximum number of health points
     * @return Maximum number of health points
     */
    int getMaxHealthPoints();

    /**
     * Method to get tank damage
     * @return Number of damage points
     */
    int getDamage();

    /**
     * Method to get number of destruction points
     * @return Number of destruction points
     */
    int getDestructionPoints();

    /**
     * Method to get number of capture points
     * @return Number of capture points
     */
    int getCapturePoints();

    /**
     * Method to get number of spawn points
     * @return Number of spawn points
     */
    Position getSpawnPosition();

    /**
     * Method to update tank in runtime
     * @param x Coordinate X
     * @param y Coordinate Y
     * @param z Coordinate Z
     * @param health Number of health points
     * @param capture_points Number of capture points
     * @param shoot_range_bonus Shoot range bonus from catapult
     */
    void update(int x, int y, int z, int health, int capture_points, int shoot_range_bonus);

    /**
     * Method to update tank position in runtime
     * @param x Coordinate X
     * @param y Coordinate Y
     * @param z Coordinate Z
     */
    void update(int x, int y, int z);

    /**
     * Method to update tank health in runtime
     * @param health Number of health points
     */
    void update(int health);

    /**
     * Method to update tank position in runtime
     * @param pos Position
     */
    void update(Position pos);

    /**
     * Ostream method
     * @param stream Stream
     * @param tank Tank
     * @return ostream
     */
    friend std::ostream &operator<<(std::ostream &stream, const Tank &tank);

    std::shared_ptr<StateMachine> current_strategy_;    // Tank strategy
    std::vector<Position> list_moves_ = {};             // Tank moves
protected:
    TankType type_;             // Tank Type
    ContentType content_type;   // Content Type
    Position pos_;              // Position
    Position spawn_pos_;        // Spawn position
    int health_points_;         // Number of health points
    int max_health_points_;     // Number of maximum health points
    int capture_points_;        // Number of capture points
    int speed_points_;          // Number of speed points
    int damage_;                // Number of damage points
    int max_shot_radius_ = 0;   // Maximum shoot radius
    int min_shot_radius_ = 0;   // Minimum shoot radius
    int shoot_range_bonus_ = 0; // Shoot range bonus
    int destruction_points_;    // Number of destruction points
    int player_id_ = -1;        // Player ID
};

