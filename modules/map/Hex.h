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

/**
 * Hex class
 */
class Hex {
public:
    Position pos;                                   // Position
    std::vector<double> danger;                     // Danger
    std::vector<double> visit;                      // Visit
    bool is_occupied = false;                       // If hex is occupied
    std::shared_ptr<Content> content = nullptr;     // Content of the hex
    std::vector<Position> neighbors;                // Neighbors
    bool visited = false;                           // If it is visited
    int visited_d = 0;                             // Visited
    Position prev = {-100, -100, -100};    // Parent hex

    /**
     * Constructor
     * @param x Coordinate X
     * @param y Coordinate Y
     * @param z Coordinate Z
     * @param content_type Content type
     * @param data Hex data
     * @param id ID
     */
    Hex(int x, int y, int z, ContentType content_type = ContentType::EMPTY, json data = json::parse("{}"), int id = -1);

    /**
     * Method to set hex
     * @param content_type Content type
     * @param data Hex data
     * @param id ID
     */
    void setHex(ContentType content_type = ContentType::EMPTY, json data = json::parse("{}"), int id = -1);

    /**
     * Method to add neighbour
     * @param pos Position
     */
    void addNeighbour(Position pos);

    /**
     * Method to get json
     * @return Json
     */
    json getJson();

    /**
     * Ostream method
     * @param stream Stream
     * @param hex Hex
     * @return ostream
     */
    friend std::ostream &operator<<(std::ostream &stream, const Hex &hex) {
        stream << "(" << hex.pos << ",..." << ")";
        return stream;
    }

    /**
     * Method to clear the hex
     */
    void clear();
};
