#include "StateHunt.h"
#include <algorithm> //std::sort
#include <iostream> //std::cout
#include <string> //std::string
#include <vector> //std::vector

StateHunt::StateHunt(std::shared_ptr<Tank> tank, std::shared_ptr<Game> game, std::shared_ptr<Param> param) : State(tank, game, param) {
}


std::string StateHunt::getType() {
    return "hunt";
}



std::string StateHunt::calculateAction() {

    //if (!game->GuaranteedKill(tank).empty()) {
    //    return shootToString(game->GuaranteedKill(tank));
    //}

    std::vector<std::shared_ptr<Hex>> positions_to_shoot = game->findSafePositionsToShoot(tank, param->tank);

    std::shared_ptr<Hex> start = game->map.getHex(tank->getPosition());
    std::shared_ptr<Hex> pos_opponent = game->map.getHex(param->tank->getPosition());
    if (!positions_to_shoot.empty()) {
        for (auto tankList: game->tanksUnderShoot(tank)) {
            for (auto tanks_under_shoot: tankList) {
                if (tanks_under_shoot->getPosition() == param->tank->getPosition()) {
                    return shootToString(tankList);
                }
            }
        }
        std::vector<std::shared_ptr<Hex>> path = game->map.findPath(start, positions_to_shoot, tank);
        if (path.size() != 0) {
            return moveToString(path[1]);
        } else {
            return "";
        }
    } else {
        std::vector<std::shared_ptr<Hex>> positions_to_move = game->findNearestSafePositions(pos_opponent);
        std::vector<std::shared_ptr<Hex>> path = game->map.findPath(start, positions_to_shoot, tank);

        if (path.size() != 0) {
            return moveToString(path[1]);
        } else {
            return "";
        }
    }
}