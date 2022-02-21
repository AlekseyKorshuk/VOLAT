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

    players.clear();
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

    //updateDanger();
    predictingBehaviorOpponentsTanks();
}


void Game::addTank(json vehicle, int vehicle_id) {

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
    for (auto player: players)
        if (player->id != idx) {
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

    int max_dist = 1;
    for (auto hex: map.hexes_map) {
        hex.second->danger.clear();
        hex.second->danger = std::vector<double>(max_dist, 0);
    }

    for (auto tank: opponent_vehicles) {
        std::queue<std::pair<std::shared_ptr<Hex>, int> > queue;
        double damage = tank->getDamage();
        int ld = -1;
        for (auto hexList: tank->getShootingHexesAreas(map)) {
            for (auto hex: hexList) {
                queue.push({hex, 0});
                hex->danger[0] += damage;
                hex->visited = true;
            }
        }
    }
}

std::vector<std::shared_ptr<Hex>>
Game::findSafePositionsToShoot(std::shared_ptr<Tank> player_tank, std::shared_ptr<Tank> opponent_tank) {
    std::vector<std::shared_ptr<Hex>> hexes;

    Hex hex_player_tank = player_tank->getPosition();
    Hex hex_opponent_tank = opponent_tank->getPosition();

    player_tank->update(hex_opponent_tank);

    for (auto hexList: player_tank->getShootingHexesAreas(map)) {
        for (auto hex: hexList) {
            if (!hex->is_occupied && hex->content->is_reacheble && hex->danger[0] == 0) {
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

        if (current_node->danger[0] == 0) {
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

std::vector<std::shared_ptr<Hex>>
Game::findSafePath(std::shared_ptr<Hex> start, std::vector<std::shared_ptr<Hex>> ends, std::shared_ptr<Tank> tank) {
    std::shared_ptr<Hex> end = nullptr;
    std::priority_queue<std::pair<std::pair<int, double>, std::shared_ptr<Hex> >> Queue;
    bool reached_end = false;
    start->visited = true;

    Hex pos_tank = tank->getPosition();

    Queue.push({{0, 0}, start});
    while (!Queue.empty() && !reached_end) {
        double danger = -Queue.top().first.second;
        int dist = -Queue.top().first.first;
        std::shared_ptr<Hex> current_node = Queue.top().second;
        Queue.pop();

        tank->update(current_node);
        std::vector<std::shared_ptr<Hex>> achievable_hexes = tank->getAchievableHexes(map);

        for (std::shared_ptr<Hex> node: achievable_hexes) {
            if (!node->is_occupied && !node->visited) {
                node->visited = true;
                double n_danger = danger;
                if (node->danger.size() > dist) {
                    n_danger += node->danger[dist];
                }
                Queue.push({{-(dist + 1), -n_danger}, node});

                node->prev = current_node;
                if (std::find(ends.begin(), ends.end(), node) != ends.end()) {
                    reached_end = true;
                    end = node;
                    break;
                }
            }
        }
    }
    tank->update(pos_tank);

    std::vector<std::shared_ptr<Hex>> route = map.traceRoute(end);
    map.clearPath();
    return route;
}

std::vector<std::shared_ptr<Tank>> Game::findTanksToShootOnArea(std::vector<std::shared_ptr<Hex>> area) {
    std::vector<std::shared_ptr<Tank>> tanks;
    for (auto hex: area)
        for (auto opponent_vehicle: opponent_vehicles)
            if (opponent_vehicle->getPosition() == *hex && !getPlayer(opponent_vehicle->getPlayerId())->is_neutral)
                tanks.push_back(opponent_vehicle);
    return tanks;
}

bool sortbysec(const std::pair<std::shared_ptr<Hex>, int> &a,
               const std::pair<std::shared_ptr<Hex>, int> &b, Hex hex) {
    if (a.second != b.second)
        return a.second < b.second;

    return a.first->getDistance(hex) < a.first->getDistance(hex);
}

std::vector<std::shared_ptr<Hex>>
Game::findSortedSafePositionsToShoot(std::shared_ptr<Tank> player_tank, std::shared_ptr<Tank> opponent_tank) {
    std::vector<std::pair<std::shared_ptr<Hex>, int>> hexes_pairs;

    Hex hex_player_tank = player_tank->getPosition();
    Hex hex_opponent_tank = opponent_tank->getPosition();

    player_tank->update(hex_opponent_tank);

    for (auto hexList: player_tank->getShootingHexesAreas(map)) {
        for (auto hex: hexList) {
            if (!hex->is_occupied && hex->content->is_reacheble) {
                hexes_pairs.emplace_back(hex, int(hex->danger[0]));
            }
        }
    }
    player_tank->update(hex_player_tank);
    std::sort(hexes_pairs.begin(), hexes_pairs.end(),
              std::bind(sortbysec, std::placeholders::_1, std::placeholders::_2, hex_player_tank));

    std::vector<std::shared_ptr<Hex>> hexes;
    hexes.reserve(hexes_pairs.size());
    for (const auto &pair: hexes_pairs)
        hexes.push_back(pair.first);
    return hexes;
}

std::vector<std::shared_ptr<Tank>> Game::canKillAndStayAlive(const std::shared_ptr<Tank> &player_tank) {
    auto position = map.getHex(player_tank->getPosition());
    std::vector<std::shared_ptr<Tank>> danger_tanks;

    for (const auto &tank: opponent_vehicles)
        for (auto hexes: tank->getShootingHexesAreas(map))
            if (std::find(hexes.begin(), hexes.end(), position) != hexes.end()) {
                danger_tanks.push_back(tank);
            }

    auto player_shootings = player_tank->getShootingHexesAreas(map);

    std::vector<std::shared_ptr<Tank>> best_shoot;
    int best_damage = INT32_MAX;
    int best_kill_points = 0;

    for (auto player_shoot: player_shootings) {
        std::vector<std::shared_ptr<Tank>> shoot;
        int damage = 0;
        int kill_points = 0;
        for (const auto &danger_tank: danger_tanks) {
            if (std::find(player_shoot.begin(), player_shoot.end(), map.getHex(danger_tank->getPosition())) !=
                player_shoot.end()) {
                shoot.push_back(danger_tank);
                if (danger_tank->getHealthPoints() - player_tank->getDamage() <= 0)
                    kill_points += danger_tank->getDestructionPoints();
                else
                    damage += danger_tank->getDamage();

            }
        }
        if (player_tank->getHealthPoints() - damage > 0 && best_damage > damage && kill_points >= best_kill_points)
            best_shoot = shoot;
    }

    return best_shoot;
}

std::shared_ptr<Tank> Game::getTankByID(int id) {
    for (auto tank: all_vehicles)
        if (tank->id == id)
            return tank;
    return nullptr;
}


void Game::predictingBehaviorOpponentsTanks() {

    for (auto hex: map.hexes) {
        hex->danger = std::vector<double>(5);
        hex->visit = std::vector<double>(5);
    }

    for (auto tank: opponent_vehicles) {
        predictingTankBehavior(tank);
    }
}


void Game::predictingTankBehavior(std::shared_ptr<Tank> tank) {

    int max_prediction_step = 3;


    std::vector<int> directionSegments;
    if (tank->list_moves_.size() < 2) {
        directionSegments = definingDirectionSegments(tank->getPosition(),
                                                      Hex(0, 0, 0));
    } else {
        directionSegments = definingDirectionSegments(tank->getPosition(),
                                                      tank->list_moves_[tank->list_moves_.size()-2]);
    }


    std::vector<std::vector<double>>  danger_map[5];
    for (int i = 0; i < 5; i++) {
        danger_map[i] = std::vector<std::vector<double>>(map.radius_ * 2 + 2, std::vector<double>(map.radius_ * 2 + 2));
    }
    std::vector<std::vector<double>>  visit_map[5];
    for (int i = 0; i < 5; i++) {
        visit_map[i] = std::vector<std::vector<double>>(map.radius_ * 2 + 2, std::vector<double>(map.radius_ * 2 + 2));
    }

    int map_radius = map.radius_ + 1;

    int step = 0;
    Hex pos_tank = tank->getPosition();

    visit_map[0][pos_tank.x + map_radius][pos_tank.y + map_radius] = 1;

    for (auto hexList: tank->getShootingHexesAreas(map)) {
        for (auto pos: hexList) {
            danger_map[0][pos->x + map_radius][pos->y + map_radius] += 1;
        }
    }


    std::queue<std::pair<std::shared_ptr<Hex>,int>> Queue;
    Queue.push({map.getHex(tank->getPosition()), 0});


    int tank_sp = tank->getSpeedPoints();
    int k = 0;
    int number_visited_hex[5] = {1, 0, 0, 0, 0};
    while(Queue.size()) {
        std::shared_ptr<Hex> current_hex = Queue.front().first;
        int current_step = Queue.front().second;

        Queue.pop();


        tank->update(current_hex);
        int num_visits_current_hex = visit_map[(current_step + tank_sp - 1) / tank_sp][current_hex->x + map_radius][current_hex->y + map_radius];
        int step = (current_step + 1 + tank_sp - 1) / tank_sp;

        if (step  != max_prediction_step) {
            for (auto pos: current_hex->neighbors) {
                std::vector<int> currentDirectionSegments = definingDirectionSegments(*current_hex, *pos);

                bool f = false;
                for (auto i: currentDirectionSegments) {
                    for (auto j: directionSegments) {
                        if (i == j) {
                            f = true;
                        }
                    }
                }

                if (!f) continue;

                number_visited_hex[step] += num_visits_current_hex;

                if (visit_map[step][pos->x + map_radius][pos->y + map_radius] == 0) {
                    Queue.push({pos, current_step + 1});
                    k++;
                }

                visit_map[step][pos->x + map_radius][pos->y + map_radius] += num_visits_current_hex;
            }
        }
    }





    for (int i = 0; i < map_radius * 2; i++) {
        for (int j = 0; j < map_radius * 2; j++) {
            bool f = false;
            for (int q = 0; q < max_prediction_step; q++) if (visit_map[q][i][j]) {
                f = 1;
                break;
            }
            if (!f) break;

            tank->update(Hex(i - map_radius, j - map_radius, 0 - i - j + 2 * map_radius));

            std::vector<std::vector<std::shared_ptr<Hex>>> shooting_hexes_areas
                                = tank->getShootingHexesAreas(map);

            for (int q = 0; q < max_prediction_step; q++) if (visit_map[q][i][j]) {
                visit_map[q][i][j] = visit_map[q][i][j] / double (number_visited_hex[q]);
                map.getHex(Hex(i - map_radius, j - map_radius, 0 - i - j + 2 * map_radius))->visit[q]
                        += visit_map[q][i][j];
            }


            for (auto hexList: shooting_hexes_areas) {
                for (auto hex: hexList) {
                    for (int q = 0; q < max_prediction_step; q++)  {
                        hex->danger[q] += visit_map[q][i][j];
                    }
                }
            }
        }
    }
    tank->update(pos_tank);
}

std::vector<int> Game::definingDirectionSegments(Hex start, Hex end) {
    std::vector<Hex> hex_directions =
            {Hex(1, 0, -1), Hex(1, -1, 0), Hex(0, -1, 1), Hex(-1, 0, 1), Hex(-1, 1, 0), Hex(0, 1, -1)};

    std::vector<std::pair<int, int>> dist;

    for (int i = 0; i < hex_directions.size(); i++) {
        Hex current_hex = hex_directions[i] + start;
        dist.push_back({end.getDistance(current_hex), i});
    }
    std::sort(dist.begin(), dist.end());

    if (dist[0].first == dist[1].second) {
        if (dist[0].second == 0 && dist[1].second == 5) {
            return {5};
        } else {
            return {dist[0].second};
        }
    } else {
        if (dist[0].second == 0) {
            return {0, 5};
        } else{
            return {dist[0].second, dist[0].second + 1};
        }
    }
}
