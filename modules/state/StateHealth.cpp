#include "StateHealth.h"
#include <vector> //std::vector

StateHealth::StateHealth(std::shared_ptr<Tank> tank, std::shared_ptr<Game> game, std::shared_ptr<Param> param) : State(
        tank,
        game,
        param) {
}


std::string StateHealth::getType() {
    return "health";
}


std::string StateHealth::calculateAction() {
    std::vector<Position> path;
    if (tank->getTankType() == TankType::HEAVY)
        path = game->findSafePath(tank->getPosition(), game->map.hard_repair, tank);
    else
        path = game->findSafePath(tank->getPosition(), game->map.light_repair, tank);

    if (!path.empty() && game->map.getHex(path[1])->danger[0] < tank->getHealthPoints())
        return moveToString(path[1]);

    return "";

}
