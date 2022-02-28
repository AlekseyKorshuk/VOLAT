#include "StateDefence.h"
#include <algorithm> //std::sort
#include <iostream> //std::cout
#include <string> //std::string
#include <vector> //std::vector


StateDefence::StateDefence(std::shared_ptr<Tank> tank, std::shared_ptr<Game> game, std::shared_ptr<Param> param)
        : State(tank, game,
                param) {
}


std::string StateDefence::getType() {
    return "defence";
}


std::string StateDefence::calculateAction() {

    std::vector<std::shared_ptr<Tank>> tanks_on_base = game->findTanksToShootOnArea(game->map.base);

    if (tanks_on_base.empty()) {
        auto path = game->map.findPath(tank->getPosition(), game->map.base, tank);
        if (!path.empty())
            return moveToString(path[1]);
        return "";
    }

    std::shared_ptr<Tank> target = tanks_on_base[0];
    for (auto tank_on_base: tanks_on_base)
        if (target->getCapturePoints() <= tank_on_base->getCapturePoints() &&
            target->getHealthPoints() <= tank_on_base->getHealthPoints() &&
            tank->getPosition().getDistance(target->getPosition()) <
            tank->getPosition().getDistance(tank_on_base->getPosition()))
            target = tank_on_base;

    auto positions = game->findSortedSafePositionsToShoot(tank, target);

    if (positions.empty())
        return "";

    if (tank->getPosition() == positions[0])
        return shootToString(std::vector<std::shared_ptr<Tank>>{target});


    auto path = game->map.findPath(tank->getPosition(), positions[0], tank);
    if (!path.empty())
        return moveToString(path[1]);
    return "";

}
