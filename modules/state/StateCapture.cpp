#include "StateCapture.h"

StateCapture::StateCapture(std::shared_ptr<Tank> tank, Game *game, Param *param) : State(tank, game, param) {
}


std::string StateCapture::getType() {
    return "capture";
}


#include <algorithm> //std::sort
#include <iostream> //std::cout
#include <string> //std::string
#include <vector> //std::vector


void remove(std::vector<Hex *> &v) {
    auto end = v.end();
    for (auto it = v.begin(); it != end; ++it) {
        end = std::remove(it + 1, end, *it);
    }

    v.erase(end, v.end());
}

std::vector<Hex *> intersection(const std::vector<Hex *> &v1,
                                std::vector<Hex *> &v2) {
    std::vector<Hex *> v3;
    for (auto hex1: v1) {
        for (auto hex2: v2) {
            if (*hex1 == *hex2) {
                v3.push_back(hex1);
                break;
            }
        }
    }

//    remove(v3);

//    set_intersection(v1.begin(),v1.end(), v2.begin(),v2.end(), std::back_inserter(v3));

    return v3;
}


bool sortbysec(const std::pair<Hex *, int> &a,
               const std::pair<Hex *, int> &b) {
    return (a.second < b.second);
}

std::vector<std::pair<Hex *, int>> calculateShootingVector(Game *game, std::shared_ptr<Tank> tank) {
    std::map<Hex *, int> shooting_map = game->map.getShootingMap(game->opponent_vehicles);


    std::vector<Hex *> possible_moves;
    for (auto hex: intersection(tank->getAchievableHexes(game->map), game->map.base)) {
        bool exist = false;
        for (auto tank: game->all_vehicles) {
            if (tank != nullptr && tank->getPosition() == *hex) {
                exist = true;
                break;
            }
        }
        if (!exist)
            possible_moves.push_back(hex);
    }


    std::vector<std::pair<Hex *, int>> sorted_hexes;
    for (auto hex: possible_moves) {
        bool exist = false;
        for (auto &it: shooting_map) {
            if (it.first == hex) {
                sorted_hexes.emplace_back(it);
                exist = true;
                break;
            }
        }
        if (!exist)
            sorted_hexes.emplace_back(std::pair<Hex *, int>(hex, 0));

    }

    std::sort(sorted_hexes.begin(), sorted_hexes.end(), sortbysec);

    return sorted_hexes;
}


std::string StateCapture::calculateAction() {
    Hex *position = game->map.getHex(tank->getPosition());
    if (std::find(game->map.base.begin(), game->map.base.end(), position) != game->map.base.end()) {
        std::vector<std::pair<Hex *, int>> possible_moves = calculateShootingVector(game, tank);
        if (!possible_moves.empty())
            return moveToString(possible_moves[0].first);
        return "";
    }

    std::vector<Hex *> path = game->map.findPath(position, game->map.base, tank);

    if (!path.empty())
        return moveToString(path[1]);

    return "";
}
