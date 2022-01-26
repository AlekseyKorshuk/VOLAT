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

    void set_base(json map_json);

    void set_map(json state);

    void clear_path();

    std::vector<Hex *> generate_empty_map(int radius);

    Hex *get_hex(const Hex &hex);

    std::vector<Hex *> find_path(Hex start, std::vector<Hex> ends);

    std::vector<Hex *> find_path(Hex *start, std::vector<Hex *> ends);

    std::vector<Hex *> find_path(Hex start, Hex end);

    std::vector<Hex *> find_path(Hex *start, Hex *end);

    std::vector<Hex *> trace_route(Hex *end);
};