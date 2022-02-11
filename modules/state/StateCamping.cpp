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

    if (position->danger[0] == 0)
        return performAggressiveAction();


    auto shoot = game->canKillAndStayAlive(tank);
    if (!shoot.empty())
        return shootToString(shoot);

    return findSafePosition();

}

std::string StateCamping::moveToBase() {
    std::vector<std::shared_ptr<Hex>> path = game->findSafePath(game->map.getHex(tank->getPosition()), game->map.base,
                                                                tank);
    if (path.size() != 0)
        return moveToString(path[1]);
    return "";
}

std::string StateCamping::findSafePosition() {
    std::vector<std::shared_ptr<Hex>> safe_positions = game->findNearestSafePositions(
            game->map.getHex(tank->getPosition()));
    std::vector<std::shared_ptr<Hex>> path = game->map.findPath(game->map.getHex(tank->getPosition()), safe_positions,
                                                                tank);
    if (path.size() != 0)
        return moveToString(path[1]);
    return "";
}

std::string StateCamping::performAggressiveAction() {
    std::vector<std::shared_ptr<Hex>> safe_positions_to_shoot;
    for (auto opponent_vehicle: game->opponent_vehicles) {
        std::vector<std::shared_ptr<Hex>> new_safe_positions = game->findSafePositionsToShoot(tank,
                                                                                              opponent_vehicle);
        safe_positions_to_shoot.insert(safe_positions_to_shoot.end(), new_safe_positions.begin(),
                                       new_safe_positions.end());
    }
    for (auto base: game->map.base)
        if (base->danger[0] == 0)
            safe_positions_to_shoot.push_back(base);


    if (!safe_positions_to_shoot.empty()) {
        std::vector<std::shared_ptr<Hex>> path = game->findSafePath(game->map.getHex(tank->getPosition()),
                                                                    safe_positions_to_shoot, tank);
        if (path.size() > 1)
            return moveToString(path[1]);

        std::vector<std::shared_ptr<Hex>> area;
        for (auto list: tank->getShootingHexesAreas(game->map))
            area.insert(area.end(), list.begin(), list.end());

        std::vector<std::shared_ptr<Tank>> tanks = game->findTanksToShootOnArea(area);
        std::sort(tanks.begin(), tanks.end(), [](const auto &lhs, const auto &rhs) {
            if (lhs->getDestructionPoints() == rhs->getDestructionPoints())
                return lhs->getHealthPoints() > rhs->getHealthPoints();
            return lhs->getDestructionPoints() > rhs->getDestructionPoints();
        });

        if (!tanks.empty())
            return shootToString(std::vector<std::shared_ptr<Tank>>{tanks[0]});

    }
    return moveToBase();
}
