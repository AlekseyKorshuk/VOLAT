#include "StateHunt.h"
#include <algorithm> //std::sort
#include <iostream> //std::cout
#include <string> //std::string
#include <vector> //std::vector

StateHunt::StateHunt(std::shared_ptr<Tank> tank, Game *game, Param *param) : State(tank, game, param) {
}


std::string StateHunt::getType() {
    return "hunt";
}



std::string StateHunt::calculateAction() {

    std::vector<Hex *> positions_to_shoot = game->findSafePositionsToShoot(tank, param->tank);

    Hex* start = game->map.getHex(tank->getPosition());
    Hex* pos_opponent = game->map.getHex(param->tank->getPosition());
    if (!positions_to_shoot.empty()) {
        if (std::find(positions_to_shoot.begin(), positions_to_shoot.end(), start)
                != positions_to_shoot.end()) {
            for (auto tankList: game->tanksUnderShoot(tank)) {
                for (auto tanks_under_shoot: tankList) {
                    if (tanks_under_shoot->getPosition() == param->tank->getPosition()) {
                        return shootToString(tankList);
                    }
                }
            }
            return "";
        } else {
            std::vector<Hex *> path = game->map.findPath(start, positions_to_shoot, tank);
            if (path.size() != 0) {
                return moveToString(path[1]);
            } else {
                return "";
            }
        }
    } else {
        std::vector<Hex *> positions_to_move = game->findNearestSafePositions(pos_opponent);
        std::vector<Hex *> path = game->map.findPath(start, positions_to_shoot, tank);

        if (path.size() != 0) {
            return moveToString(path[1]);
        } else {
            return "";
        }
    }
}
