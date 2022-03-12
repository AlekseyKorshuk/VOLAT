#include "StateCamping.h"

StateCamping::StateCamping(std::shared_ptr<Tank> tank, std::shared_ptr<Game> game, std::shared_ptr<Param> param)
        : State(tank,
                game,
                param) {
}


std::string StateCamping::getType() {
    return "camping";
}

std::string StateCamping::calculateAction() {
    std::shared_ptr<Hex> position = game->map.getHex(tank->getPosition());

    if (position->danger[0] != 0) {
        return performAggressiveAction();
    } else {
        auto safe_base = game->getSafePositions(tank, game->map.base, true, false);
        if (!safe_base.empty()) {
            auto path = game->smartFindQuickPath(tank->getPosition(), safe_base,
                                           tank);
            if (!path.empty())
                return moveToString(path[1]);
        }
        std::string action = game->getSafeShootAction(tank);
        if (!action.empty())
            return action;
    }

    auto shoot = game->canKillAndStayAlive(tank);
    if (!shoot.empty())
        return shootToString(shoot);

    return findSafePosition();
}


std::string StateCamping::moveToBase() {
    auto path = game->findSafePath(tank->getPosition(), game->map.base,
                                   tank);
    if (path.size() != 0)
        return moveToString(path[1]);
    return "";
}

std::string StateCamping::findSafePosition() {
    auto safe_positions = game->findNearestSafePositions(
            tank->getPosition());

    if (std::find(safe_positions.begin(), safe_positions.end(), tank->getPosition()) != safe_positions.end()) {
        return "";
    }

    auto path = game->smartFindQuickPath(tank->getPosition(), safe_positions,
                                   tank);
    if (path.size() != 0)
        return moveToString(path[1]);
    return "";
}

std::string StateCamping::performAggressiveAction() {
    auto shoot = game->canKillAndStayAlive(tank);
    if (!shoot.empty())
        return shootToString(shoot);

    auto action = findSafePosition();
    if (!action.empty())
        return action;

    auto possible_shoots = game->getPossibleShoots(tank);
    if (!possible_shoots.empty())
        return shootToString(game->selectBestShoot(possible_shoots, tank, false));

    return "";
}
