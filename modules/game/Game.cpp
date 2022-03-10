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
    current_game_state = state_json;
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
            int shoot_range_bonus = vehicle["shoot_range_bonus"].get<std::int32_t>();


            updateTank(vehicle_id, x, y, z, health, capture_points, shoot_range_bonus);
        }
    }

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

    for (const auto &player: players) {
        player->update(state_json);
    }

    std::shared_ptr<Player> me = getPlayer(idx);
    for (const auto &player: players)
        if (player->id != idx) {
            player->is_neutral = true;
            if (std::find(me->whoAttacked.begin(), me->whoAttacked.end(), player->id) != me->whoAttacked.end()) {
                player->is_neutral = false;
            } else {
                int k = 0;
                for (auto i: player->whoAttacked) {
                    if (i != idx) k++;
                }
                if (k == 0) {
                    player->is_neutral = false;
                }
            }
        }
}

void Game::addPlayer(const json &player_json) {
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

void Game::updateTank(int id, int x, int y, int z, int health, int capture_points, int shoot_range_bonus) {
    std::shared_ptr<Tank> tank = all_vehicles[id - 1];

    if (tank != nullptr) {
        map.changeOccupied(tank->getPosition(), false);
        tank->update(x, y, z, health, capture_points, shoot_range_bonus);
        map.changeOccupied(tank->getPosition(), true);
    }
}

std::vector<std::vector<std::shared_ptr<Tank> > > Game::tanksUnderShoot(const std::shared_ptr<Tank> &tank) {
    std::vector<std::vector<std::shared_ptr<Tank> > > tanks;

    auto shootingHexesAreas = tank->getShootingHexesAreas(map);

    for (const auto &hexPtrList: shootingHexesAreas) {
        for (const auto &pos: hexPtrList) {
            std::vector<std::shared_ptr<Tank> > tankList;
            for (const auto &opponent_vehicle: opponent_vehicles) {
                if (pos == opponent_vehicle->getPosition()
                    && opponent_vehicle->getHealthPoints() != 0
                    && !getPlayer(opponent_vehicle->getPlayerId())->is_neutral) {
                    tankList.push_back(opponent_vehicle);
                }
            }
            if (!tankList.empty()) {
                tanks.push_back(tankList);
            }
        }
    }

    return tanks;
}


std::vector<Position>
Game::findSafePositionsToShoot(const std::shared_ptr<Tank> &player_tank, const std::shared_ptr<Tank> &opponent_tank) {
    std::vector<Position> hexes;

    Position pos_player_tank = player_tank->getPosition();
    Position pos_opponent_tank = opponent_tank->getPosition();

    player_tank->update(pos_opponent_tank);

    for (const auto &hexList: player_tank->getShootingHexesAreas(map)) {
        for (const auto &pos: hexList) {
            std::shared_ptr<Hex> hex = map.getHex(pos);
            if (!hex->is_occupied && hex->content->is_reacheble && hex->danger[0] == 0) {
                hexes.push_back(pos);
            }
        }
    }

    player_tank->update(pos_player_tank);
    return hexes;
}

std::vector<Position> Game::findNearestSafePositions(const Position &start) {

    std::vector<Position> hexes;

    std::vector<Position> visited_positions;
    std::queue<Position> Queue;
    Queue.push(start);


    bool reached_end = false;

    while (!Queue.empty()) {
        Position current_pos = Queue.front();

        if (map.getHex(current_pos)->danger[0] == 0) {
            reached_end = true;
            hexes.push_back(current_pos);
        }
        Queue.pop();

        if (!reached_end) {
            for (const Position &pos: map.getHex(current_pos)->neighbors) {
                std::shared_ptr<Hex> node = map.getHex(pos);
                if (!node->visited) {
                    if (node->content != nullptr && node->content->is_reacheble) {
                        node->visited = true;
                        visited_positions.push_back(pos);
                        Queue.push(pos);
                    }
                }
            }
        }
    }

    map.clearPath(visited_positions);
    return hexes;
}

std::vector<std::shared_ptr<Tank>> Game::GuaranteedKill(const std::shared_ptr<Tank> &tank) {
    std::vector<std::vector<std::shared_ptr<Tank> > > tanks_under_shoot = tanksUnderShoot(tank);

    int max_kill_points = 0;
    int q = -1;
    for (int i = 0; i < tanks_under_shoot.size(); i++) {
        int current_kill_points = 0;

        for (const auto &tank_opponent: tanks_under_shoot[i]) {
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

std::vector<Position>
Game::findSafePath(const Position &start, std::vector<Position> ends, const std::shared_ptr<Tank> &tank) {
    Position end = {-100, -100, -100};
    std::priority_queue<std::pair<std::pair<int, double>, Position >> Queue;
    bool reached_end = false;
    std::vector<Position> visited_positions;

    visited_positions.push_back(start);
    map.getHex(start)->visited = true;

    Position pos_tank = tank->getPosition();

    Queue.push({{0, 0}, start});
    while (!Queue.empty() && !reached_end) {
        double danger = -Queue.top().first.second;
        int dist = -Queue.top().first.first;
        Position current_pos = Queue.top().second;
        Queue.pop();

        tank->update(current_pos);
        std::vector<Position> achievable_hexes = tank->getAchievableHexes(map);

        for (const Position &pos: achievable_hexes) {
            std::shared_ptr<Hex> node = map.getHex(pos);
            if (!node->is_occupied && !node->visited) {
                node->visited = true;
                visited_positions.push_back(pos);

                double n_danger = danger;
                if (node->danger.size() > dist) {
                    n_danger += node->danger[dist];
                }
                Queue.push({{-(dist + 1), -n_danger}, pos});

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

    std::vector<Position> route = map.traceRoute(end);
    map.clearPath(visited_positions);
    return route;
}

std::vector<std::shared_ptr<Tank>>
Game::findTanksToShootOnArea(const std::vector<Position> &area) {
    std::vector<std::shared_ptr<Tank>> tanks;

    for (auto pos: area)
        for (const auto &opponent_vehicle: opponent_vehicles)
            if (opponent_vehicle->getPosition() == pos && !getPlayer(opponent_vehicle->getPlayerId())->is_neutral)
                tanks.push_back(opponent_vehicle);
    return tanks;
}

bool sortbysec(const std::pair<Position, int> &a,
               const std::pair<Position, int> &b, Position pos) {
    if (a.second != b.second)
        return a.second < b.second;

    return a.first.getDistance(pos) < a.first.getDistance(pos);
}

std::vector<Position>
Game::findSortedSafePositionsToShoot(const std::shared_ptr<Tank> &player_tank,
                                     const std::shared_ptr<Tank> &opponent_tank) {
    std::vector<std::pair<Position, int>> hexes_pairs;

    Position pos_player_tank = player_tank->getPosition();
    Position pos_opponent_tank = opponent_tank->getPosition();

    player_tank->update(pos_opponent_tank);

    for (const auto &posList: player_tank->getShootingHexesAreas(map)) {
        for (const auto &pos: posList) {
            std::shared_ptr<Hex> hex = map.getHex(pos);
            if (!hex->is_occupied && hex->content->is_reacheble) {
                hexes_pairs.emplace_back(pos, int(hex->danger[0]));
            }
        }
    }
    player_tank->update(pos_player_tank);
    std::sort(hexes_pairs.begin(), hexes_pairs.end(),
              [pos_player_tank](auto &&PH1, auto &&PH2) {
                  return sortbysec(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2), pos_player_tank);
              });

    std::vector<Position> hexes;
    hexes.reserve(hexes_pairs.size());
    for (const auto &pair: hexes_pairs)
        hexes.push_back(pair.first);

    return hexes;
}

std::vector<std::shared_ptr<Tank>> Game::canKillAndStayAlive(const std::shared_ptr<Tank> &player_tank) {
    auto position = player_tank->getPosition();
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
            if (std::find(player_shoot.begin(), player_shoot.end(), danger_tank->getPosition()) !=
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

    for (const auto &hex: map.hexes) {
        hex->danger = std::vector<double>(5);
        hex->visit = std::vector<double>(5);
    }

    for (const auto &tank: opponent_vehicles) {
        predictingTankBehavior(tank);
    }
}


void Game::predictingTankBehavior(const std::shared_ptr<Tank> &tank) {


    int max_prediction_step = 3;


    std::vector<int> directionSegments;
    if (tank->list_moves_.size() < 2) {
        directionSegments = definingDirectionSegments(tank->getPosition(),
                                                      Position(0, 0, 0));
    } else {
        directionSegments = definingDirectionSegments(tank->getPosition(),
                                                      tank->list_moves_[tank->list_moves_.size() - 2]);
    }

    std::vector<std::vector<double>> danger_map[5];
    for (auto &i: danger_map) {
        i = std::vector<std::vector<double>>(map.radius_ * 2 + 2, std::vector<double>(map.radius_ * 2 + 2));
    }
    std::vector<std::vector<double>> visit_map[5];
    for (auto &i: visit_map) {
        i = std::vector<std::vector<double>>(map.radius_ * 2 + 2, std::vector<double>(map.radius_ * 2 + 2));
    }

    int map_radius = map.radius_ + 1;

    int step = 0;
    Position pos_tank = tank->getPosition();

    visit_map[0][pos_tank.getX() + map_radius][pos_tank.getY() + map_radius] = 1;
    /*
    for (const auto &hexList: tank->getShootingHexesAreas(map)) {
        for (const auto &pos: hexList) {
            danger_map[0][pos.getX() + map_radius][pos.getY() + map_radius] += 1;
        }
    }
    */


    std::queue<std::pair<Position, int>> Queue;
    Queue.push({tank->getPosition(), 0});


    int tank_sp = tank->getSpeedPoints();
    int k = 0;
    int number_visited_hex[5] = {1, 0, 0, 0, 0};
    while (!Queue.empty()) {
        Position current_pos = Queue.front().first;
        int current_step = Queue.front().second;

        Queue.pop();


        tank->update(current_pos);
        int num_visits_current_hex = visit_map[(current_step + tank_sp - 1) / tank_sp][current_pos.getX() + map_radius][
                current_pos.getY() + map_radius];
        int step = (current_step + 1 + tank_sp - 1) / tank_sp;

        if (step != max_prediction_step) {
            for (auto pos: map.getHex(current_pos)->neighbors) {
                std::vector<int> currentDirectionSegments = definingDirectionSegments(current_pos, pos);

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

                if (visit_map[step][pos.getX() + map_radius][pos.getY() + map_radius] == 0) {
                    Queue.push({pos, current_step + 1});
                    k++;
                }

                visit_map[step][pos.getX() + map_radius][pos.getY() + map_radius] += num_visits_current_hex;
            }
        }
    }


    for (int i = 0; i < map_radius * 2; i++) {
        for (int j = 0; j < map_radius * 2; j++) {
            bool f = false;
            for (int q = 0; q < max_prediction_step; q++)
                if (visit_map[q][i][j] != 0) {
                    f = true;
                    break;
                }
            if (!f) continue;

            tank->update(Position(i - map_radius, j - map_radius, 0 - i - j + 2 * map_radius));

            std::vector<std::vector<Position>> shooting_hexes_areas
                    = tank->getShootingHexesAreas(map);

            for (int q = 0; q < max_prediction_step; q++)
                if (visit_map[q][i][j]) {
                    visit_map[q][i][j] = visit_map[q][i][j] / double(number_visited_hex[q]);
                    map.getHex(Position(i - map_radius, j - map_radius, 0 - i - j + 2 * map_radius))->visit[q]
                            += visit_map[q][i][j];
                }


            for (const auto &hexList: shooting_hexes_areas) {
                for (auto pos: hexList) {
                    for (int q = 0; q < max_prediction_step; q++) {
                        map.getHex(pos)->danger[q] += visit_map[q][i][j];
                    }
                }
            }
        }
    }
    tank->update(pos_tank);
}

std::vector<int> Game::definingDirectionSegments(Position start, Position end) {
    std::vector<Position> hex_directions =
            {Position(1, 0, -1), Position(1, -1, 0), Position(0, -1, 1), Position(-1, 0, 1), Position(-1, 1, 0),
             Position(0, 1, -1)};

    std::vector<std::pair<int, int>> dist;

    for (int i = 0; i < hex_directions.size(); i++) {
        Position current_hex = hex_directions[i] + start;
        dist.emplace_back(end.getDistance(current_hex), i);
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
        } else {
            return {dist[0].second, dist[0].second + 1};
        }
    }
}

bool Game::isDefenceNeeded(const std::shared_ptr<Tank> &player_tank) {
    auto position = player_tank->getPosition();
    auto player_id = player_tank->getPlayerId();
    json players_on_base;
    for (auto player: current_game_state["players"]) {
        players_on_base[std::to_string(player["idx"].get<std::int32_t>())]["tanks_on_base"] = 0;
        int capture_points = current_game_state["win_points"][std::to_string(
                player["idx"].get<std::int32_t>())]["capture"].get<std::int32_t>();
        players_on_base[std::to_string(player["idx"].get<std::int32_t>())]["capture_points"] = capture_points;
    }

    int total_num_vehicles_on_base = 0;
    for (const auto &vehicle: all_vehicles) {
        if (vehicle == nullptr) continue;
        if (std::find(map.base.begin(), map.base.end(), vehicle->getPosition()) != map.base.end()) {
            int value = players_on_base[std::to_string(vehicle->getPlayerId())]["tanks_on_base"].get<std::int32_t>();

            players_on_base[std::to_string(vehicle->getPlayerId())]["tanks_on_base"] = value + 1;
            total_num_vehicles_on_base++;
        }
    }

    if (players_on_base[std::to_string(player_id)]["tanks_on_base"].get<std::int32_t>() == 0 &&
        total_num_vehicles_on_base != 0)
        return true;
    return false;
}


bool Game::isCaptureNeeded(const std::shared_ptr<Tank> &player_tank) {
//    std::cout << player_tank->getStringTankType() << std::endl;

    for (const auto &position: map.base) {
        auto hex = map.getHex(position);
//        std::cout << *hex << " " << hex->danger[0] << std::endl;
        if (hex->danger[0] < player_tank->getHealthPoints() && (!hex->is_occupied || player_tank->getPosition() == position))
            return true;
    }

    return false;
}

std::vector<Position>
Game::getSafePositions(const std::shared_ptr<Tank> &player_tank, std::vector<Position> area, bool is_zero_danger) {
    std::vector<Position> list;
    for (const auto &position: area) {
        auto hex = map.getHex(position);
        if (is_zero_danger) {
            if (hex->danger[0] == 0)
                list.push_back(position);
        } else {
            if (hex->danger[0] < player_tank->getHealthPoints())
                list.push_back(position);
        }
    }

    if (!is_zero_danger)
        sort(list.begin(), list.end(), [this](const Position &lhs, const Position &rhs) {
            return map.getHex(lhs)->danger[0] > map.getHex(rhs)->danger[0];
        });

    return list;
}

std::string Game::getSafeShootAction(const std::shared_ptr<Tank>& player_tank) {
    std::vector<std::vector<std::shared_ptr<Tank>>> shoots;
    std::vector<Position> moves;
    for (const auto &opponent_vehicle: opponent_vehicles) {
        auto positions = findSortedSafePositionsToShoot(player_tank, opponent_vehicle);

        if (player_tank->getPosition() == positions[0])
            shoots.push_back(std::vector<std::shared_ptr<Tank>>{opponent_vehicle});

        auto path = findSafePath(player_tank->getPosition(), positions, player_tank);
        if (!path.empty())
            moves.push_back(path[1]);
    }

    if (!shoots.empty()) {
        return player_tank->current_strategy_->getState()->shootToString(selectBestShoot(shoots, player_tank, false));
    }

    if (!moves.empty())
        return player_tank->current_strategy_->getState()->moveToString(selectBestMove(moves, player_tank));

    return "";
}

std::vector<std::shared_ptr<Tank>>
Game::selectBestShoot(std::vector<std::vector<std::shared_ptr<Tank>>> shoots, const std::shared_ptr<Tank> &player_tank,
                      bool stay_alive = false) {
    if (shoots.empty())
        return std::vector<std::shared_ptr<Tank>>(0);
    auto best_shoot = shoots[0];
    int best_damage = -1, best_kill_points = -1;
    for (const auto &shoot: shoots) {
        int damage = 0, kill_points = 0;
        double income_damage = map.getHex(player_tank->getPosition())->danger[0];
        for (const auto &tank: shoot) {
            if (!getPlayer(tank->getPlayerId())->is_neutral) {
                damage += std::max(player_tank->getDamage(), tank->getHealthPoints());
                if (player_tank->getDamage() >= tank->getHealthPoints()) {
                    kill_points += tank->getDestructionPoints();
                    income_damage -= tank->getDamage();
                }
            }
        }
        if (kill_points >= best_kill_points && damage > best_damage) {
            bool add = true;
            if (stay_alive)
                if (player_tank->getHealthPoints() <= income_damage)
                    add = false;
            if (add) {
                best_shoot = shoot;
                best_damage = damage;
                best_kill_points = kill_points;
            }

        }
    }
    return best_shoot;
}

Position Game::selectBestMove(std::vector<Position> moves, std::shared_ptr<Tank> player_tank) {
    Position best_move = moves[0];
    double best_danger = map.getHex(best_move)->danger[0];
    for (const auto &move: moves) {
        auto hex = map.getHex(move);
        if (hex->danger[0] < best_danger) {
            best_danger = hex->danger[0];
            best_move = move;
        }
    }
    return best_move;
}

std::vector<std::vector<std::shared_ptr<Tank>>> Game::getPossibleShoots(const std::shared_ptr<Tank> &player_tank) {
    auto shooting_hexes = player_tank->getShootingHexesAreas(map);
    std::vector<std::vector<std::shared_ptr<Tank>>> shoots;
    for (auto hexes: shooting_hexes) {
        std::vector<std::shared_ptr<Tank>> shoot;
        for (auto hex: hexes) {
            for (auto opponent_vehicle: opponent_vehicles) {
                if (opponent_vehicle->getPosition() == hex && !getPlayer(opponent_vehicle->getPlayerId())->is_neutral) {
                    shoot.push_back(opponent_vehicle);
                }
            }
        }
        if (!shoot.empty())
            shoots.push_back(shoot);
    }
    return shoots;
}

std::vector<std::shared_ptr<Tank>>
Game::selectBestShootDefence(std::vector<std::vector<std::shared_ptr<Tank>>> shoots,
                             const std::shared_ptr<Tank> &player_tank) {
    if (shoots.empty())
        return std::vector<std::shared_ptr<Tank>>(0);
    auto best_shoot = shoots[0];
    int best_damage = -1, best_kill_points = -1, best_defence_points = 0;
    for (const auto &shoot: shoots) {
        int damage = 0, kill_points = 0, defence_points = 0;
        double income_damage = map.getHex(player_tank->getPosition())->danger[0];
        for (const auto &tank: shoot) {
            if (std::find(map.base.begin(), map.base.end(), tank->getPosition()) != map.base.end() &&
                !getPlayer(tank->getPlayerId())->is_neutral) {
                damage += std::max(player_tank->getDamage(), tank->getHealthPoints());
                if (player_tank->getDamage() >= tank->getHealthPoints()) {
                    kill_points += tank->getDestructionPoints();
                    income_damage -= tank->getDamage();
                    defence_points += tank->getCapturePoints();
                }
            }
        }
        if (defence_points > best_defence_points) {
            best_shoot = shoot;
            best_damage = damage;
            best_kill_points = kill_points;
            best_defence_points = defence_points;
        }
    }
    return best_shoot;
}

