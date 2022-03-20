#pragma once

#include <iostream>
#include <queue>
#include <list>
#include <map>
#include "Hex.h"


#include <nlohmann/json.hpp>

using json = nlohmann::json;


class Tank;

/**
 * Map class
 */
class Map {
public:
    int radius_;                                                // Radius
    std::vector<std::shared_ptr<Hex>> hexes;                    // Hexes
    std::vector<std::vector<std::shared_ptr<Hex>>> hexes_map;   // Map of hexes
    std::vector<Position> base;                                 // Base
    std::vector<Position> light_repair;                         // Light repair
    std::vector<Position> hard_repair;                          // Hard repair
    std::vector<Position> obstacle;                             // Obstacle
    int num_visited = 0;                                        // Number of visited

    /**
     * Constructor
     * @param map_json Map in json format
     * @param radius Radius
     */
    Map(json map_json, int radius = 10);

    /**
     * Map destructor
     */
    Map() {};

    /**
     * Method to set base
     * @param map_json Map json
     */
    void setBase(json map_json);

    /**
     * Set spawn points
     * @param map_json
     */
    void setSpawnPoints(json map_json);

    /**
     * Method to set obstacles
     * @param map_json Map json
     */
    void setObstacle(json map_json);

    /**
     * Method to set catapult
     * @param map_json Map json
     */
    void setCatapult(json map_json);

    /**
     * Method to set light repairs
     * @param map_json Map json
     */
    void setLightRepair(json map_json);

    /**
     * Method to set hard repairs
     * @param map_json Map json
     */
    void setHardRepair(json map_json);

    /**
     * Method to change occupied positions
     * @param pos Position
     * @param is_occupied If position is occupied
     */
    void changeOccupied(const Position &pos, bool is_occupied);

    /**
     * Method to clear path
     * @param posList Positions
     */
    void clearPath(std::vector<Position> posList);

    /**
     * Method to check if belongs
     * @param h Position
     * @return Boolean result
     */
    bool belongs(const Position &h) const;

    /**
     * Method to generate map
     * @param radius Radius
     * @return Resulting vector
     */
    std::vector<std::shared_ptr<Hex>> generateEmptyMap(int radius);

    /**
     * Method to get Hex
     * @param pos Position
     * @return Hex
     */
    std::shared_ptr<Hex> getHex(const Position &pos) const;

    /**
     * Method to find path
     * @param start Starting position
     * @param ends Target positions
     * @return Path
     */
    std::vector<Position> findPath(Position start, std::vector<Position> ends, std::shared_ptr<Tank>);

    /**
     * Method to find path
     * @param start Starting position
     * @param end Target position
     * @return Path
     */
    std::vector<Position> findPath(Position start, Position end, std::shared_ptr<Tank>);

    /**
     * Method to trace route
     * @param end Target position
     * @return Path
     */
    std::vector<Position> traceRoute(Position end);
};

#include "../content/vehicles/Tank.h"