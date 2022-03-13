#pragma once

#include <iostream>
#include <queue>
#include<list>
#include<map>
#include "Hex.h"


#include <nlohmann/json.hpp>

using json = nlohmann::json;


class Tank;

class Map {
public:
    int radius_;

    std::vector<std::shared_ptr<Hex>> hexes;
    std::vector<std::vector<std::shared_ptr<Hex>>> hexes_map;

    std::vector<Position> base;
    std::vector<Position> light_repair;
    std::vector<Position> hard_repair;
    std::vector<Position> obstacle;

    int num_visited = 0;

    Map(json map_json, int radius = 10);

    Map() {};

    void setBase(json map_json);

    void setSpawnPoints(json map_json);

    void setObstacle(json map_json);

    void setCatapult(json map_json);

    void setLightRepair(json map_json);

    void setHardRepair(json map_json);


    void changeOccupied(const Position &, bool);

    void clearPath(std::vector<Position>);

    bool belongs(const Position &h) const;

    std::vector<std::shared_ptr<Hex>> generateEmptyMap(int radius);

    std::shared_ptr<Hex> getHex(const Position &pos) const;

    std::vector<Position> findPath(Position start, std::vector<Position> ends, std::shared_ptr<Tank>);

    std::vector<Position> findPath(Position start, Position end, std::shared_ptr<Tank>);

    std::vector<Position> traceRoute(Position end);

    //std::map<Position, int> getShootingMap(std::vector<std::shared_ptr<Tank>> opponent_vehicles);
};

#include "../content/vehicles/Tank.h"