#include "Map.h"



Map::Map(json map_json, int radius) : radius_(radius) {
    hexes = generateEmptyMap(radius);
    setBase(map_json);
}

void Map::setBase(json map_json) {
    for (auto hex: map_json["content"]["base"])
        base.push_back(
                getHex(
                        Hex(
                                hex["x"].get<std::int32_t>(),
                                hex["y"].get<std::int32_t>(),
                                hex["z"].get<std::int32_t>()
                        )
                )
        );
}

void Map::changeOccupied(Hex hex, bool is_occupied) {
    getHex(hex)->is_occupied = is_occupied;
}

void Map::clearPath() {
    for (auto hex: hexes) {
        hex->visited = false;
        hex->prev = nullptr;
    }
}

std::vector<Hex *> Map::generateEmptyMap(int radius) {
    std::vector<Hex> deltas = {Hex(1, 0, -1), Hex(0, 1, -1), Hex(-1, 1, 0), Hex(-1, 0, 1), Hex(0, -1, 1),
                               Hex(1, -1, 0)};
    hexes_map.clear();
    hexes.clear();
    std::vector<int> v;
    for (int r = 0; r < radius + 1; r++) {
        int x = 0;
        int y = -1 * r;
        int z = r;
        v = {x, y, z};
        Hex *hex = new Hex(x, y, z);
        hexes_map.insert(std::pair<std::vector<int>, Hex *>(v, hex));
        hexes.push_back(hex);

        for (int j = 0; j < 6; j++) {
            int num_of_hexes_in_edge = 0;
            if (j == 5)
                num_of_hexes_in_edge = r - 1;
            else
                num_of_hexes_in_edge = r;

            for (int i = 0; i < num_of_hexes_in_edge; i++) {
                x = x + deltas[j].x;
                y = y + deltas[j].y;
                z = z + deltas[j].z;
                v = {x, y, z};
                hex = new Hex(x, y, z);
                hexes_map.insert(std::pair<std::vector<int>, Hex *>(v, hex));
                hexes.push_back(hex);
            }
        }
    }
    for (auto itr = hexes_map.begin(); itr != hexes_map.end(); ++itr) {
        Hex *hex = itr->second;
        for (int i = 0; i < 6; i++) {
            Hex neighbor_temp = hex->getNeighbor(i);
            if (neighbor_temp.getLength() <= 10) {
                v = {neighbor_temp.x, neighbor_temp.y, neighbor_temp.z};
                Hex *neighbor = hexes_map.find(v)->second;
                hex->addNeighbour(neighbor);
            }
        }
    }

    return hexes;
}

bool Map::belongs(const Hex& h) const {
    return h.getLength() <= radius_;
}

Hex *Map::getHex(const Hex &hex) const {
    std::vector<int> v1 = {hex.x, hex.y, hex.z};
    return this->hexes_map.find(v1)->second;
}

std::vector<Hex *> Map::findPath(Hex start, std::vector<Hex> ends, std::shared_ptr<Tank> tank) {
    // If path does not exist, will be returned list only with the "END" Hex
    std::vector<Hex *> ends_vector;
    for (auto hex: ends)
        ends_vector.push_back(this->getHex(hex));
    return this->findPath(this->getHex(start), ends_vector, tank);
}

std::vector<Hex *> Map::findPath(Hex *start, std::vector<Hex *> ends, std::shared_ptr<Tank> tank) {
    Hex *end = nullptr;
    // If path does not exist, will be returned list only with the "END" Hex
    std::queue<Hex *> Queue;
    bool reached_end = false;
    start->visited = true;
    Queue.push(start);
    while (!Queue.empty() && !reached_end) {
        Hex *current_node = Queue.front();
        Queue.pop();

        std::vector<Hex*> achievable_hexes = tank->getAchievableHexes(*this);

        for (Hex *node: achievable_hexes) {
            if (!node->is_occupied) {
                node->visited = true;
                Queue.push(node);

                node->prev = current_node;
                if (std::find(ends.begin(), ends.end(), node) != ends.end()) {
                    reached_end = true;
                    end = node;
                    break;
                }
            }
        }
    }

    std::vector<Hex *> route = traceRoute(end);
    this->clearPath();
    return route;
}

std::vector<Hex *> Map::findPath(Hex start, Hex end, std::shared_ptr<Tank> tank) {
    // If path does not exist, will be returned list only with the "END" Hex
    return this->findPath(this->getHex(start), this->getHex(end), tank);
}

std::vector<Hex *> Map::findPath(Hex *start, Hex *end, std::shared_ptr<Tank> tank) {
    // If path does not exist, will be returned list only with the "END" Hex

    std::vector<Hex*> ends = {end};
    return this->findPath(start, ends, tank);
}

std::vector<Hex *> Map::traceRoute(Hex *end) {
    std::list<Hex *> route;
    Hex *node = end;
    while (node != nullptr) {
        route.push_front(node);
        node = node->prev;
    }
    std::vector<Hex *> v{std::begin(route), std::end(route)};
    return v;
}