#pragma once

#include <iostream>
#include <queue>
#include<list>
#include<map>
#include "Hex.h"


#include <nlohmann/json.hpp>

using json = nlohmann::json;


class Map {
public:

    std::vector<Hex *> hexes;
    std::map<std::vector<int>, Hex *> hexes_map;
    std::vector<Hex *> player_vehicles;
    std::vector<Hex *> opponent_vehicles;
    std::vector<Hex *> base;

    Map(json map_json, int radius = 10);

    void setBase(json map_json);

    void setMap(json state);

    void clearPath();

    std::vector<Hex *> generateEmptyMap(int radius);

    Hex *getHex(const Hex &hex);

    std::vector<Hex *> findPath(Hex start, std::vector<Hex> ends);

    std::vector<Hex *> findPath(Hex *start, std::vector<Hex *> ends);

    std::vector<Hex *> findPath(Hex start, Hex end);

    std::vector<Hex *> findPath(Hex *start, Hex *end);

    std::vector<Hex *> traceRoute(Hex *end);
};