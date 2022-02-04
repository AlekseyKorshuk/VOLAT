#include "StateCapture.h"

StateCapture::StateCapture(std::shared_ptr<Tank> tank, Game *game) : State(tank, game) {

}


std::string StateCapture::getType() {
    return "capture";
}


#include <algorithm> //std::sort
#include <iostream> //std::cout
#include <string> //std::string
#include <vector> //std::vector


void remove(std::vector<Hex*> &v) {
    auto end = v.end();
    for (auto it = v.begin(); it != end; ++it) {
        end = std::remove(it + 1, end, *it);
    }

    v.erase(end, v.end());
}

std::vector<Hex *> intersection(const std::vector<Hex *>& v1,
                                std::vector<Hex *>& v2) {
    std::vector<Hex *> v3;
    for (auto hex1: v1) {
        if(std::find_if(v2.begin(), v2.end(), [&hex1](Hex* my) { return my->getJson() == hex1->getJson(); }) != v2.end()){
            v3.push_back(hex1);
        }
    }

    remove(v3);

    return v3;
}


bool sortbysec(const std::pair<Hex*,int> &a,
               const std::pair<Hex*,int> &b)
{
    return (a.second < b.second);
}

std::vector<std::pair<Hex *, int>> calculateShootingVector(Game *game, std::shared_ptr<Tank> tank) {
    std::map<Hex *, int> shooting_map;
    for (const auto &opponent_vehicle: game->opponent_vehicles) {
        for (auto list: opponent_vehicle->getShootingHexesAreas(game->map)) {
            for (auto hex: list) {
                auto value = shooting_map.find(hex);
                if (value == shooting_map.end()) {
                    shooting_map.insert(std::pair<Hex *, int>(hex, 1));
                } else {
                    value->second++;
                }
            }
        }
    }

    std::vector<std::pair<Hex *, int>> sorted_hexes;

    std::cout << "getAchievableHexes: ";
    for (auto h: tank->getAchievableHexes(game->map))
        std::cout << *h << " ";
    std::cout << std::endl;

    std::cout << "Base: ";
    for (auto h: game->map.base)
        std::cout << *h << " ";
    std::cout << std::endl;

    std::vector<Hex *> possible_moves = intersection(tank->getAchievableHexes(game->map), game->map.base);


    for (auto &it: shooting_map) {
        if(std::find_if(possible_moves.begin(), possible_moves.end(), [&it](Hex* hex) { return hex->getJson() == it.first->getJson(); }) != possible_moves.end()){
            sorted_hexes.emplace_back(it);
        }
    }
    std::cout << '\n';

    std::cout << "NOT sorted_hexes: ";
    for (auto h: sorted_hexes)
        std::cout << *h.first << "-" << h.second << " ";
    std::cout << std::endl;
    std::sort(sorted_hexes.begin(), sorted_hexes.end(), sortbysec);

    std::cout << "Sorted_hexes: ";
    for (auto h: sorted_hexes)
        std::cout << *h.first << "-" << h.second << " ";
    std::cout << std::endl;

    return sorted_hexes;

}

Hex *getPossibleMove(std::vector<Hex> tank_moves, std::vector<Hex *> possible_moves) {
    for (auto possible_move: possible_moves)
        for (auto tank_move: tank_moves)
            if (*possible_move == tank_move)
                return possible_move;

    return nullptr;
}

std::string StateCapture::calculateAction() {
    Hex *position = game->map.getHex(tank->getPosition());
    std::cout << "OPPONENT: " << game->opponent_vehicles.size() << '\n';
    if (std::find(game->map.base.begin(), game->map.base.end(), position) != game->map.base.end()) {
        std::cout << "Already on base! " << *position << '\n';
        std::vector<std::pair<Hex *, int>> possible_moves = calculateShootingVector(game, tank);

        possible_moves.erase(std::remove_if(possible_moves.begin(), possible_moves.end(), [&position](std::pair<Hex *, int> p){ return p.first->getJson() == position->getJson();}), possible_moves.end());

        if (!possible_moves.empty())
            return moveToString(possible_moves[0].first);
        return "";

    }

    std::vector<Hex *> path = game->map.findPath(position, game->map.base, tank);

    if (!path.empty())
        return moveToString(path[1]);

    return "";


}
