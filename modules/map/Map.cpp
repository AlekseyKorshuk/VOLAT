#include "Map.h"


Map::Map(json map_json, int radius) {
    hexes = generate_empty_map(radius);
    set_base(map_json);
}

void Map::set_base(json map_json) {
    for (auto hex: map_json["content"]["base"])
        base.push_back(
                get_hex(
                        Hex(
                                hex["x"].get<std::int32_t>(),
                                hex["y"].get<std::int32_t>(),
                                hex["z"].get<std::int32_t>()
                        )
                )
        );
}

void Map::set_map(json state) {
    player_vehicles.clear();
    int player_id = state["current_player_idx"].get<std::int32_t>();
    for (auto it = state["vehicles"].begin(); it != state["vehicles"].end(); ++it) {
        int vehicle_id = stoi(it.key());
        json vehicle = it.value();
        json position = vehicle["position"];
        Hex *hex = this->get_hex(Hex(position["x"], position["y"], position["z"]));
        hex->id = vehicle_id;
        hex->is_occupied = true;
        hex->data = vehicle;
//        std::cout << *hex << std::endl;
        if (player_id == vehicle["player_id"].get<std::int32_t>())
            player_vehicles.push_back(hex);
        else
            opponent_vehicles.push_back(hex);
        /**
         {
          "player_id": 1,
          "vehicle_type": "medium_tank",
          "health": 2,
          "spawn_position": {
            "x": -7,
            "y": -3,
            "z": 10
          },
          "position": {
            "x": -7,
            "y": -3,
            "z": 10
          },
          "capture_points": 0
        }
         */
    }
}

void Map::clear_path() {
    for (auto hex: hexes) {
        hex->visited = false;
        hex->prev = nullptr;
    }
}

std::vector<Hex *> Map::generate_empty_map(int radius) {
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
            Hex neighbor_temp = hex->hex_neighbor(i);
            if (neighbor_temp.hex_length() <= 10) {
                v = {neighbor_temp.x, neighbor_temp.y, neighbor_temp.z};
                Hex *neighbor = hexes_map.find(v)->second;
                hex->addNeighbour(neighbor);
            }
        }
    }

    return hexes;
}

Hex *Map::get_hex(const Hex &hex) {
    std::vector<int> v1 = {hex.x, hex.y, hex.z};
    return this->hexes_map.find(v1)->second;
}

std::vector<Hex *> Map::find_path(Hex start, std::vector<Hex> ends) {
    // If path does not exist, will be returned list only with the "END" Hex
    std::vector<Hex *> ends_vector;
    for (auto hex: ends)
        ends_vector.push_back(this->get_hex(hex));
    return this->find_path(this->get_hex(start), ends_vector);
}

std::vector<Hex *> Map::find_path(Hex *start, std::vector<Hex *> ends) {
    Hex *end = nullptr;
    // If path does not exist, will be returned list only with the "END" Hex
    std::queue<Hex *> Queue;
    bool reached_end = false;
    start->visited = true;
    Queue.push(start);

    while (!Queue.empty() && !reached_end) {
        Hex *current_node = Queue.front();
        Queue.pop();
        for (Hex *node: current_node->neighbors) {
            if (!node->visited && !node->is_occupied) {
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
    std::vector<Hex *> route = trace_route(end);
    this->clear_path();
    return route;
}


std::vector<Hex *> Map::find_path(Hex start, Hex end) {
    // If path does not exist, will be returned list only with the "END" Hex
    return this->find_path(this->get_hex(start), this->get_hex(end));
}

std::vector<Hex *> Map::find_path(Hex *start, Hex *end) {
    // If path does not exist, will be returned list only with the "END" Hex
    std::queue<Hex *> Queue;
    bool reached_end = false;
    start->visited = true;
    Queue.push(start);

    while (!Queue.empty() && !reached_end) {
        Hex *current_node = Queue.front();
        Queue.pop();
        for (Hex *node: current_node->neighbors) {
            if (!node->visited && !node->is_occupied) {
                node->visited = true;
                Queue.push(node);
                node->prev = current_node;
                if (node == end) {
                    reached_end = true;
                    break;
                }
            }
        }
    }
    std::vector<Hex *> route = trace_route(end);
    this->clear_path();
    return route;
}

std::vector<Hex *> Map::trace_route(Hex *end) {
    std::list<Hex *> route;
    Hex *node = end;
    while (node != nullptr) {
        route.push_front(node);
        node = node->prev;
    }
    std::vector<Hex *> v{std::begin(route), std::end(route)};
    return v;
}