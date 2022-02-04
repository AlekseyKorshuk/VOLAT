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
    std::vector<Hex *> hexes;
    std::map<std::vector<int>, Hex *> hexes_map;
    std::vector<Hex *> player_vehicles;
    std::vector<Hex *> opponent_vehicles;
    std::vector<Hex *> base;

    Map(json map_json, int radius = 10);

    Map() {};

    void setBase(json map_json);


    void changeOccupied(Hex, bool);

    void clearPath();

    bool belongs(const Hex& h) const;

    std::vector<Hex *> generateEmptyMap(int radius);

    Hex *getHex(const Hex &hex) const;

    std::vector<Hex *> findPath(Hex start, std::vector<Hex> ends, std::shared_ptr<Tank>);

    std::vector<Hex *> findPath(Hex *start, std::vector<Hex *> ends, std::shared_ptr<Tank>);

    std::vector<Hex *> findPath(Hex start, Hex end, std::shared_ptr<Tank>);

    std::vector<Hex *> findPath(Hex *start, Hex *end, std::shared_ptr<Tank>);

    std::vector<Hex *> traceRoute(Hex *end);

};

#include "../content/vehicles/Tank.h"