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
    std::map<std::vector<int>, std::shared_ptr<Hex>> hexes_map;
    std::vector<std::shared_ptr<Hex>> base;

    Map(json map_json, int radius = 10);

    Map() {};

    void setBase(json map_json);

    void setSpawnPoints(json map_json);

    void changeOccupied(const Hex&, bool);

    void clearPath();

    bool belongs(const Hex &h) const;

    std::vector<std::shared_ptr<Hex>> generateEmptyMap(int radius);

    std::shared_ptr<Hex> getHex(const Hex &hex) const;

    std::vector<std::shared_ptr<Hex>> findPath(Hex start, std::vector<Hex> ends, std::shared_ptr<Tank>);

    std::vector<std::shared_ptr<Hex>>
    findPath(std::shared_ptr<Hex> start, std::vector<std::shared_ptr<Hex>> ends, std::shared_ptr<Tank>);

    std::vector<std::shared_ptr<Hex>> findPath(Hex start, Hex end, std::shared_ptr<Tank>);

    std::vector<std::shared_ptr<Hex>>
    findPath(std::shared_ptr<Hex> start, std::shared_ptr<Hex> end, std::shared_ptr<Tank>);

    std::vector<std::shared_ptr<Hex>> traceRoute(std::shared_ptr<Hex> end);

    std::map<std::shared_ptr<Hex>, int> getShootingMap(std::vector<std::shared_ptr<Tank>> opponent_vehicles);
};

#include "../content/vehicles/Tank.h"