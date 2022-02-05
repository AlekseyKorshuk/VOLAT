#include "Game.h"
#include "../content/vehicles/MediumTank.h"
#include "../content/vehicles/AtSpg.h"
#include "../content/vehicles/HeavyTank.h"
#include "../content/vehicles/LightTank.h"
#include "../content/vehicles/Spg.h"

Game::Game(int idx, json map_json, json state_json) : idx(idx) {
    map = Map(map_json);

    for (int i = 0; i < 15; i++) all_vehicles.push_back(nullptr);

    json players_json = state_json["players"];
    for (json::iterator it = players_json.begin(); it != players_json.end(); ++it) {
        json player_json = it.value();
        addPlayer(player_json);
    }


    for (auto it = state_json["vehicles"].begin(); it != state_json["vehicles"].end(); ++it) {
        int vehicle_id = stoi(it.key());
        json vehicle = it.value();
        addTank(vehicle, vehicle_id);
    }
}

void Game::update(json state_json) {
    updatePlayers(state_json);

    for (auto it = state_json["vehicles"].begin(); it != state_json["vehicles"].end(); ++it) {
        int vehicle_id = stoi(it.key());
        json vehicle = it.value();

        if (all_vehicles[vehicle_id - 1] == nullptr) {
            addTank(vehicle, vehicle_id);
        } else {
            json position = vehicle["position"];

            int x = vehicle["position"]["x"].get<std::int32_t>();
            int y = vehicle["position"]["y"].get<std::int32_t>();
            int z = vehicle["position"]["z"].get<std::int32_t>();

            int capture_points = vehicle["capture_points"].get<std::int32_t>();
            int health = vehicle["health"].get<std::int32_t>();

            updateTank(vehicle_id, x, y, z, health, capture_points);
        }
    }

    updateDanger();
}


void Game::addTank(json vehicle, int vehicle_id) {
    json position = vehicle["position"];
    json spawn_position = vehicle["spawn_position"];

    int x = vehicle["position"]["x"].get<std::int32_t>();
    int y = vehicle["position"]["y"].get<std::int32_t>();
    int z = vehicle["position"]["z"].get<std::int32_t>();

    int spawn_x = vehicle["spawn_position"]["x"].get<std::int32_t>();
    int spawn_y = vehicle["spawn_position"]["y"].get<std::int32_t>();
    int spawn_z = vehicle["spawn_position"]["z"].get<std::int32_t>();

    std::string temp_vehicle_type = vehicle["vehicle_type"].get<std::string>();

    std::shared_ptr<Tank> tank = nullptr;
    if (temp_vehicle_type == "at_spg") {
        tank = std::make_shared<AtSpg>(
                x, y, z,
                spawn_x, spawn_y, spawn_z,
                vehicle["health"].get<std::int32_t>(),
                vehicle["capture_points"].get<std::int32_t>(), vehicle_id
        );
        tank->setPlayerId(vehicle["player_id"].get<std::int32_t>());
    }
    if (temp_vehicle_type == "heavy_tank") {
        tank = std::make_shared<HeavyTank>(
                x, y, z,
                spawn_x, spawn_y, spawn_z,
                vehicle["health"].get<std::int32_t>(),
                vehicle["capture_points"].get<std::int32_t>(), vehicle_id
        );
        tank->setPlayerId(vehicle["player_id"].get<std::int32_t>());
    }
    if (temp_vehicle_type == "light_tank") {
        tank = std::make_shared<LightTank>(
                x, y, z,
                spawn_x, spawn_y, spawn_z,
                vehicle["health"].get<std::int32_t>(),
                vehicle["capture_points"].get<std::int32_t>(), vehicle_id
        );
        tank->setPlayerId(vehicle["player_id"].get<std::int32_t>());
    }
    if (temp_vehicle_type == "medium_tank") {
        tank = std::make_shared<MediumTank>(
                x, y, z,
                spawn_x, spawn_y, spawn_z,
                vehicle["health"].get<std::int32_t>(),
                vehicle["capture_points"].get<std::int32_t>(), vehicle_id
        );
        tank->setPlayerId(vehicle["player_id"].get<std::int32_t>());
    }
    if (temp_vehicle_type == "spg") {
        tank = std::make_shared<Spg>(
                x, y, z,
                spawn_x, spawn_y, spawn_z,
                vehicle["health"].get<std::int32_t>(),
                vehicle["capture_points"].get<std::int32_t>(), vehicle_id
        );
        tank->setPlayerId(vehicle["player_id"].get<std::int32_t>());
    }

    all_vehicles[vehicle_id - 1] = tank;


    if (tank != nullptr) {
        map.changeOccupied(tank->getPosition(), true);

        if (vehicle["player_id"].get<std::int32_t>() == idx) {
            player_vehicles.push_back(tank);
        } else {
            opponent_vehicles.push_back(tank);
        }

        getPlayer(tank->getPlayerId())->vehicles.push_back(tank);
    }
}

void Game::updatePlayers(json state_json) {
    json players_json = state_json["players"];

    for (json::iterator it = players_json.begin(); it != players_json.end(); ++it) {
        json player_json = it.value();
        if (getPlayer(player_json["idx"].get<std::int32_t>()) != nullptr) {
            addPlayer(player_json);
        }
    }

    for (auto player: players) {
        player->update(state_json);
    }

    std::shared_ptr<Player> me = getPlayer(idx);
    for (auto player: players) if (player->id != idx) {
        player->is_neutral = true;
        if (std::find(me->whoAttacked.begin(), me->whoAttacked.end(), player->id) != me->whoAttacked.end()) {
            player->is_neutral = false;
        } else {
            int k = 0;
            for (auto i: player->onWhomAttacked) {
                if (i != idx) k++;
            }
            if (k == 0) {
                player->is_neutral = false;
            }
        }
    }
}

void Game::addPlayer(json player_json) {
    players.push_back(std::make_shared<Player>(player_json));
}

std::shared_ptr<Player> Game::getPlayer(int id) {
    for (int i = 0; i < players.size(); i++)
        if (players[i]->id == id) {
            return players[i];
        }
    return nullptr;
}

void Game::updateTank(int id, int x, int y, int z) {
    std::shared_ptr<Tank> tank = all_vehicles[id - 1];

    if (tank != nullptr) {
        map.changeOccupied(tank->getPosition(), false);
        tank->update(x, y, z);
        map.changeOccupied(tank->getPosition(), true);
    }
}

void Game::updateTank(int id, int x, int y, int z, int health, int capture_points) {
    std::shared_ptr<Tank> tank = all_vehicles[id - 1];

    if (tank != nullptr) {
        map.changeOccupied(tank->getPosition(), false);
        tank->update(x, y, z, health, capture_points);
        map.changeOccupied(tank->getPosition(), true);
    }
}

std::vector<std::vector<std::shared_ptr<Tank> > > Game::tanksUnderShoot(std::shared_ptr<Tank> tank) {
    std::vector<std::vector<std::shared_ptr<Tank> > > tanks;

    auto shootingHexesAreas = tank->getShootingHexesAreas(map);

    for (auto hexPtrList: shootingHexesAreas) {
        for (auto hex: hexPtrList) {
            std::vector<std::shared_ptr<Tank> > tankList;
            for (auto tank: opponent_vehicles) {
                if (*hex == tank->getPosition()
                    && tank->getHealthPoints() != 0
                    && !getPlayer(tank->getPlayerId())->is_neutral) {
                    tankList.push_back(tank);
                }
            }
            if (!tankList.empty()) {
                tanks.push_back(tankList);
            }
        }
    }

    return tanks;
}

void Game::updateDanger() {
    map_danger.clear();
    for (auto hex: map.hexes_map) {
        map_danger.insert({hex.first, 0});
    }


    for (auto tank: opponent_vehicles) {
        std::vector<HexPtrList> shooting_hexes_areas = tank->getShootingHexesAreas(map);
        for (auto hexList: shooting_hexes_areas) {
            for (auto hex: hexList) {
                std::vector<int> pos = {hex->x, hex->y, hex->z};
                if (map_danger.find(pos) != map_danger.end()) {
                    map_danger[pos] += tank->getDamage();
                }
            }
        }
    }
}

std::vector<std::shared_ptr<Hex>> Game::findSafePositionsToShoot(std::shared_ptr<Tank> player_tank, std::shared_ptr<Tank> opponent_tank) {
    std::vector<std::shared_ptr<Hex>> hexes;

    Hex hex_player_tank = player_tank->getPosition();
    Hex hex_opponent_tank = opponent_tank->getPosition();

    player_tank->update(hex_opponent_tank);

    for (auto hexList: player_tank->getShootingHexesAreas(map)) {
        for (auto hex: hexList) {
            std::vector<int> pos = {hex->x, hex->y, hex->z};
            if (!hex->is_occupied && hex->content->is_reacheble && map_danger[pos] == 0) {
                hexes.push_back(hex);
            }
        }
    }

    player_tank->update(hex_player_tank);
    return hexes;
}

std::vector<std::shared_ptr<Hex>> Game::findNearestSafePositions(std::shared_ptr<Hex> start) {

    std::vector<std::shared_ptr<Hex>> hexes;

    std::queue<std::shared_ptr<Hex>> Queue;
    Queue.push(start);


    bool reached_end = false;

    while (!Queue.empty()) {
        std::shared_ptr<Hex> current_node = Queue.front();

        std::vector<int> pos = {current_node->x, current_node->y, current_node->z};
        if (map_danger[pos] == 0) {
            reached_end = true;
            hexes.push_back(current_node);
        }
        Queue.pop();

        if (!reached_end) {
            for (std::shared_ptr<Hex> node: current_node->neighbors)
                if (!node->visited) {
                    if (node->content != nullptr && node->content->is_reacheble) {
                        node->visited = true;
                        Queue.push(node);
                    }
                }
        }
    }

    map.clearPath();
    return hexes;
}

std::vector<std::shared_ptr<Tank>> Game::GuaranteedKill(std::shared_ptr<Tank> tank) {
    std::vector<std::vector<std::shared_ptr<Tank> > > tanks_under_shoot = tanksUnderShoot(tank);

    int max_kill_points = 0;
    int q = -1;
    for (int i = 0; i < tanks_under_shoot.size(); i++) {
        int current_kill_points = 0;

        for (auto tank_opponent: tanks_under_shoot[i]) {
            if (tank_opponent->getHealthPoints() == tank->getDamage()) {
                current_kill_points += tank_opponent->getDestructionPoints();
            }
        }

        if (current_kill_points > max_kill_points) {
            max_kill_points = current_kill_points;
            q = i;
        }
    }
    if (q != -1)
        return tanks_under_shoot[q];

    return std::vector<std::shared_ptr<Tank>>();
}