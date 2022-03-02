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
        auto safe_base = game->getSafePositions(tank, game->map.base, true);
        if (!safe_base.empty()) {
            auto path = game->findSafePath(tank->getPosition(), safe_base,
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
    auto path = game->map.findPath(tank->getPosition(), safe_positions,
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

    std::vector<std::vector<std::shared_ptr<Tank>>> shoots;
    for (auto list: tank->getShootingHexesAreas(game->map)){
        std::vector<std::shared_ptr<Tank>> shoot;
        for (auto position: list){
            for (const auto &opponent_vehicle: game->opponent_vehicles)
                if (opponent_vehicle->getPosition() == position && !game->getPlayer(opponent_vehicle->getPlayerId())->is_neutral)
                    shoot.push_back(opponent_vehicle);

        }
    }
    auto best_shoot = game->selectBestShoot(shoots, tank, false);
    if (!best_shoot.empty())
        return shootToString(best_shoot);
    return "";
}
