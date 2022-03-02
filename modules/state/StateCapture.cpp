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
        // Already on base
        return onBaseAction();
    }
    // Not on base
    auto base_positions = game->getSafePositions(tank, game->map.base, false);
    if (!base_positions.empty()) {
        auto path = game->map.findPath(tank->getPosition(), base_positions,
                                       tank);
        if (!path.empty())
            return moveToString(path[1]);
    }

    std::string action = game->getSafeShootAction(tank);
    if (!action.empty())
        return action;

    auto path = game->findSafePath(position, game->map.base, tank);

    if (!path.empty())
        return moveToString(path[1]);

    auto possible_shoots = game->getPossibleShoots(tank);
    if (!possible_shoots.empty())
        return shootToString(game->selectBestShoot(possible_shoots, tank, false));

    return "";

}

std::string StateCapture::onBaseAction() {
    if (game->map.getHex(tank->getPosition())->danger[0] > 0) {
        // in danger
//        std::cout << "in danger! ";
        auto shoot = game->selectBestShoot(game->getPossibleShoots(tank), tank, true);
        if (!shoot.empty())
            return shootToString(shoot);
        auto safe_position = game->getSafePositions(tank, game->map.base, true);
        if (!safe_position.empty()) {
            auto path = game->map.findPath(tank->getPosition(), safe_position,
                                           tank);
            if (!path.empty())
                return moveToString(path[1]);
        }
    }
//    std::cout << "in safe position OR the only way is to shoot! ";
    if (game->current_game_state["current_turn"] == 19)
        std::cout << game->map.getHex(tank->getPosition())->danger[0];

    // in safe position OR the only way is to shoot
    auto possible_shoots = game->getPossibleShoots(tank);
    std::cout << possible_shoots.size();
    if (!possible_shoots.empty())
        return shootToString(game->selectBestShoot(possible_shoots, tank, false));
    return "";
}

