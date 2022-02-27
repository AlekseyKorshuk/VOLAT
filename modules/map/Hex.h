#pragma once

#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <nlohmann/json.hpp>
#include <vector>
#include "../content/Content.h"
#include "Position.h"

using json = nlohmann::json;

class Hex {
public:
    Position pos;

    std::vector<double> danger;
    std::vector<double> visit;

    bool is_occupied = false;

    std::shared_ptr<Content> content = nullptr;

    std::vector<Position> neighbors;

    bool visited = false;
    int  visited_d = 0;
    Position prev = {-100, -100, -100};

    Hex(int x, int y, int z, ContentType content_type = ContentType::EMPTY, json data = json::parse("{}"), int id = -1);

    void setHex(ContentType content_type = ContentType::EMPTY, json data = json::parse("{}"), int id = -1);

    void addNeighbour(Position pos);

    json getJson();

    friend std::ostream &operator<<(std::ostream &stream, const Hex &hex) {
        stream << "(" << hex.pos << ",..." << ")";
        return stream;
    }

    void clear();


};
