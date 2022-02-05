#pragma once

#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <nlohmann/json.hpp>
#include <vector>
#include "../content/Content.h"

using json = nlohmann::json;

class Hex {
public:
    int x;
    int y;
    int z;
    bool is_occupied = false;
    std::shared_ptr<Content> content = nullptr;
    std::vector<std::shared_ptr<Hex>> neighbors;
    bool visited = false;
    std::shared_ptr<Hex> prev = nullptr;

    Hex(int x, int y, int z, ContentType content_type = ContentType::EMPTY, json data = json::parse("{}"), int id = -1);

    void setHex(ContentType content_type = ContentType::EMPTY, json data = json::parse("{}"), int id = -1);

    bool operator<(const Hex &other) const;

    Hex &operator+=(const Hex &hex);

    friend Hex operator+(Hex &a, const Hex &b);

    Hex &operator-=(const Hex &hex);

    friend Hex operator-(Hex &a, const Hex &b);

    Hex getDirection(int direction);

    Hex getNeighbor(int direction);

    Hex getDiagonalNeighbor(int direction);

    int getLength() const;

    int getDistance(Hex hex);

    void addNeighbour(std::shared_ptr<Hex> hex);

    json getJson();

    friend std::ostream &operator<<(std::ostream &stream, const Hex &hex) {
        stream << "(" << hex.x << "," << hex.y << "," << hex.z << ")";
        return stream;
    }

    bool operator==(Hex hex);

    void clear();


};
