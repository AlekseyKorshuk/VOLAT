#include "Map.h"


Map::Map(json map_json, int radius) : radius_(radius) {
    hexes = generateEmptyMap(radius);
    setBase(map_json);
    setSpawnPoints(map_json);
    setObstacle(map_json);
    setLightRepair(map_json);
    setHardRepair(map_json);
    setCatapult(map_json);
}

void Map::setBase(json map_json) {
    for (auto hex: map_json["content"]["base"])
        base.push_back(
                Position(
                        hex["x"].get<std::int32_t>(),
                        hex["y"].get<std::int32_t>(),
                        hex["z"].get<std::int32_t>()));

    for (auto pos: base) {
        getHex(pos)->setHex(ContentType::BASE);
    }
}

void Map::setSpawnPoints(json map_json) {
    json spawn_points = map_json["spawn_points"];

    for (auto it = spawn_points.begin(); it != spawn_points.end(); ++it) {
        json player = it.value();
        for (auto it = player.begin(); it != player.end(); ++it) {
            json vehicle = it.value();
            for (auto pos: vehicle) {
                Position position = Position(pos["x"].get<std::int32_t>(),
                                             pos["y"].get<std::int32_t>(),
                                             pos["z"].get<std::int32_t>());
                getHex(position)->setHex(ContentType::SPAWN_POINT);
            }
        }
    }
}

void Map::setObstacle(json map_json) {
    json obstacles = map_json["content"]["obstacle"];

    for (auto obstacle_ = obstacles.begin(); obstacle_ != obstacles.end(); ++obstacle_) {
        json pos = obstacle_.value();
        Position position = Position(pos["x"].get<std::int32_t>(),
                                     pos["y"].get<std::int32_t>(),
                                     pos["z"].get<std::int32_t>());
        obstacle.push_back(position);
        getHex(position)->setHex(ContentType::OBSTACLE);
    }
}


void Map::changeOccupied(const Position &pos, bool is_occupied) {
    getHex(pos)->is_occupied = is_occupied;
}

void Map::setLightRepair(json map_json) {
    json objects = map_json["content"]["light_repair"];
    for (auto object = objects.begin(); object != objects.end(); ++object) {
        json pos = object.value();
        Position hex = Position(pos["x"].get<std::int32_t>(),
                                pos["y"].get<std::int32_t>(),
                                pos["z"].get<std::int32_t>());
        light_repair.push_back(hex);
        getHex(hex)->setHex(ContentType::LIGHT_RAPAIR);
    }
}

void Map::setHardRepair(json map_json) {
    json objects = map_json["content"]["hard_repair"];

    for (auto object = objects.begin(); object != objects.end(); ++object) {
        json pos = object.value();
        Position hex = Position(pos["x"].get<std::int32_t>(),
                                pos["y"].get<std::int32_t>(),
                                pos["z"].get<std::int32_t>());
        hard_repair.push_back(hex);
        getHex(hex)->setHex(ContentType::HARD_REPAIR);
    }
}

void Map::setCatapult(json map_json) {
    json objects = map_json["content"]["catapult"];

    for (auto object = objects.begin(); object != objects.end(); ++object) {
        json pos = object.value();
        Position hex = Position(pos["x"].get<std::int32_t>(),
                                pos["y"].get<std::int32_t>(),
                                pos["z"].get<std::int32_t>());
        getHex(hex)->setHex(ContentType::CATAPULT);
    }
}


void Map::clearPath(std::vector<Position> posList) {
    for (auto pos: posList) {
        std::shared_ptr<Hex> hex = getHex(pos);
        hex->visited = false;
        hex->prev = {-100, -100, -100};
    }
}

std::vector<std::shared_ptr<Hex>> Map::generateEmptyMap(int radius) {
    std::vector<Position> deltas = {Position(1, 0, -1), Position(0, 1, -1), Position(-1, 1, 0),
                                    Position(-1, 0, 1), Position(0, -1, 1), Position(1, -1, 0)};
    hexes_map.clear();
    hexes.clear();

    hexes_map = std::vector<std::vector<std::shared_ptr<Hex>>>
            (radius * 2 + 2, std::vector<std::shared_ptr<Hex>>(radius * 2 + 2));

    for (int r = 0; r < radius + 1; r++) {
        int x = 0;
        int y = -1 * r;
        int z = r;

        std::shared_ptr<Hex> hex = std::make_shared<Hex>(x, y, z);
        hexes_map[x + radius_ + 1][y + radius_ + 1] = hex;
        hexes.push_back(hex);

        for (int j = 0; j < 6; j++) {
            int num_of_hexes_in_edge = 0;
            if (j == 5)
                num_of_hexes_in_edge = r - 1;
            else
                num_of_hexes_in_edge = r;

            for (int i = 0; i < num_of_hexes_in_edge; i++) {
                x = x + deltas[j].getX();
                y = y + deltas[j].getY();
                z = z + deltas[j].getZ();

                hex = std::make_shared<Hex>(x, y, z);
                hexes_map[x + radius_ + 1][y + radius_ + 1] = hex;
                hexes.push_back(hex);

            }
        }
    }

    for (auto itr: hexes) {
        std::shared_ptr<Hex> hex = itr;
        for (int i = 0; i < 6; i++) {
            Position neighbor_temp = hex->pos.getNeighbor(i);
            if (neighbor_temp.getLength() <= 10) {
                int x = neighbor_temp.getX();
                int y = neighbor_temp.getY();

                Position neighbor = hexes_map[x + radius_ + 1][y + radius_ + 1]->pos;
                hex->addNeighbour(neighbor);
            }
        }
    }

    return hexes;
}

bool Map::belongs(const Position &h) const {
    return h.getLength() <= radius_;
}

std::shared_ptr<Hex> Map::getHex(const Position &pos) const {
    if (pos.getX() == -100 || !belongs(pos)) {
        return nullptr;
    }

    return hexes_map[pos.getX() + radius_ + 1][pos.getY() + radius_ + 1];
}

std::vector<Position>
Map::findPath(Position start, std::vector<Position> ends, std::shared_ptr<Tank> tank) {
    Position end = {-100, -100, -100};

    std::vector<Position> visited_positions;
    std::queue<std::pair<Position, int>> Queue;

    bool reached_end = false;
    visited_positions.push_back(start);
    getHex(start)->visited = true;

    Position pos_tank = tank->getPosition();

    Queue.push({start, 0});

    while (!Queue.empty() && !reached_end) {
        Position current_pos = Queue.front().first;
        int current_dist = Queue.front().second + 1;
        Queue.pop();


        tank->update(current_pos);
        std::vector<Position> achievable_hexes = tank->getAchievableHexes(*this);

        std::sort(achievable_hexes.begin(), achievable_hexes.end(),
                  [&current_pos](Position a, Position b) {
                      return a.getDistance(current_pos) > b.getDistance(current_pos);
                  });

        for (Position pos: achievable_hexes) {
            std::shared_ptr<Hex> node = getHex(pos);
            if (!node->is_occupied && !node->visited && node->content->is_reacheble) {
                visited_positions.push_back(pos);
                node->visited = true;
                Queue.push({pos, current_dist});

                node->prev = current_pos;
                if (std::find(ends.begin(), ends.end(), pos) != ends.end()) {
                    reached_end = true;
                    end = pos;
                    break;
                }
            }
        }
    }

    tank->update(pos_tank);

    std::vector<Position> route = traceRoute(end);
    this->clearPath(visited_positions);
    return route;
}

std::vector<Position> Map::findPath(Position start, Position end, std::shared_ptr<Tank> tank) {
    // If path does not exist, will be returned list only with the "END" Hex
    std::vector<Position> ends = {end};
    return this->findPath(start, ends, tank);
}


std::vector<Position> Map::traceRoute(Position end) {
    std::list<Position> route;
    std::shared_ptr<Hex> node = getHex(end);
    while (node != nullptr) {
        route.push_front(node->pos);
        if (node->pos == node->prev) break;
        node = getHex(node->prev);
    }
    std::vector<Position> v{std::begin(route), std::end(route)};
    return v;
}

/*
std::map<std::shared_ptr<Hex>, int> Map::getShootingMap(std::vector<std::shared_ptr<Tank>> opponent_vehicles) {
    std::map<std::shared_ptr<Hex>, int> shooting_map;
    for (const auto &opponent_vehicle: opponent_vehicles) {
        for (auto list: opponent_vehicle->getShootingHexesAreas(*this)) {
            for (auto hex: list) {
                auto value = shooting_map.find(hex);
                if (value == shooting_map.end()) {
                    shooting_map.insert(std::pair<std::shared_ptr<Hex>, int>(hex, 1));
                } else {
                    value->second++;
                }
            }
        }
    }
    return shooting_map;
}
*/

