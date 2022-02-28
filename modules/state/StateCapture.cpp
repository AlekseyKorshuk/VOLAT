#include "StateCapture.h"
#include <algorithm> //std::sort
#include <vector> //std::vector


StateCapture::StateCapture(std::shared_ptr<Tank> tank, std::shared_ptr<Game> game, std::shared_ptr<Param> param)
        : State(tank, game,
                param) {
}


std::string StateCapture::getType() {
    return "capture";
}


std::vector<Position> intersection(const std::vector<Position> &v1,
                                   std::vector<Position> &v2) {
    std::vector<Position> v3;
    for (auto hex1: v1) {
        for (auto hex2: v2) {
            if (hex1 == hex2) {
                v3.push_back(hex1);
                break;
            }
        }
    }

    return v3;
}


bool sortbysec(const std::pair<Position, int> &a,
               const std::pair<Position, int> &b) {
    return (a.second < b.second);
}

std::vector<std::pair<Position, int>>
calculateShootingVector(std::shared_ptr<Game> game, std::shared_ptr<Tank> tank) {


    std::vector<Position> possible_moves;
    for (auto hex: intersection(tank->getAchievableHexes(game->map), game->map.base)) {
        bool exist = false;
        for (auto tank: game->all_vehicles) {
            if (tank != nullptr && tank->getPosition() == hex) {
                exist = true;
                break;
            }
        }
        if (!exist)
            possible_moves.push_back(hex);
    }

    possible_moves.push_back(tank->getPosition());

    std::vector<std::pair<Position, int>> sorted_hexes;
    sorted_hexes.reserve(possible_moves.size());
    for (const auto &hex: possible_moves) {
        sorted_hexes.emplace_back(hex, game->map.getHex(hex)->danger[0]);
    }

    std::sort(sorted_hexes.begin(), sorted_hexes.end(), sortbysec);

    return sorted_hexes;
}

bool checkPosition(std::vector<std::pair<Position, int>> moves, Position position) {
    for (auto move: moves)
        if (move.first == position && move.second != moves[0].second)
            return true;
        else
            return false;
    return false;
}

std::string StateCapture::calculateAction() {
    auto position = tank->getPosition();

    if (std::find(game->map.base.begin(), game->map.base.end(), position) != game->map.base.end()) {
        auto shoot = game->canKillAndStayAlive(tank);
        if (!shoot.empty())
            return shootToString(shoot);

        std::vector<std::pair<Position, int>> possible_moves = calculateShootingVector(game, tank);
        if (!possible_moves.empty() && possible_moves[0].first != position)
            if (checkPosition(possible_moves, position))
                return moveToString(possible_moves[0].first);

        return shootAction();
    }

    auto path = game->map.findPath(position, game->map.base, tank);

    if (!path.empty())
        return moveToString(path[1]);
    return shootAction();

}

std::string StateCapture::shootAction() {
    std::vector<std::shared_ptr<Tank>> tanks = game->GuaranteedKill(tank);
    if (!tanks.empty())
        return shootToString(tanks);

    auto shooting_hexes = tank->getShootingHexesAreas(game->map);
    std::vector<std::shared_ptr<Tank>> best_shoot;
    int best_damage = -1;
    for (auto hexes: shooting_hexes) {
        std::vector<std::shared_ptr<Tank>> shoot;
        int damage = 0;
        for (auto hex: hexes) {
            for (auto opponent_vehicle: game->opponent_vehicles) {
                if (opponent_vehicle->getPosition() == hex &&
                    !game->getPlayer(opponent_vehicle->getPlayerId())->is_neutral) {
                    shoot.push_back(opponent_vehicle);
                    damage += std::max(opponent_vehicle->getHealthPoints(), tank->getDamage());
                }
            }
        }
        if (!shoot.empty() && best_damage < damage) {
            best_damage = damage;
            best_shoot = shoot;
        }
    }

    if (!best_shoot.empty())
        return shootToString(best_shoot);

    return "";
}

