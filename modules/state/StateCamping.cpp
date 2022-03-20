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
    auto action_save = game->saveTeam(tank);
    if (!action_save.empty())
        return action_save;

    std::shared_ptr<Hex> position = game->map.getHex(tank->getPosition());
    if (position->danger[0] != 0) {
        return performAggressiveAction();
    } else {
        auto shoot = game->selectBestShoot(game->getPossibleShoots(tank, true), tank, true);
        if (!shoot.empty())
            return shootToString(shoot);

        std::string action = game->getSafeShootAction(tank);
        if (!action.empty()) {
            return action;
        }




        auto position = tank->getPosition();
        json players_on_base = game->getCaptureState();
        auto player_id = tank->getPlayerId();

        int total_num_vehicles_on_base = 0;
        for (const auto &vehicle: game->all_vehicles) {
            if (vehicle == nullptr) continue;
            if (std::find(game->map.base.begin(), game->map.base.end(), vehicle->getPosition()) != game->map.base.end()) {
                total_num_vehicles_on_base++;
            }
        }

        std::vector<Position> posit;
        if (players_on_base[std::to_string(player_id)]["tanks_on_base"].get<std::int32_t>() > 1 &&
            total_num_vehicles_on_base - players_on_base[std::to_string(player_id)]["tanks_on_base"].get<std::int32_t>() <= 1)
            posit = game->map.base;
        else{
            posit = game->getHexesByRadius(2);
        }

        auto safe_base = game->getSafePositions(tank, posit, true, false);
        if (!safe_base.empty()) {
            auto path = game->smartFindSafePath(tank->getPosition(), safe_base,
                                                tank);
            if (!path.empty())
                return moveToString(path[1]);
        }

    }

    auto shoot = game->canKillAndStayAlive(tank);
    if (!shoot.empty())
        return shootToString(shoot);

    return "";
    return findSafePosition();
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

bool sortbydistance2(const Position &a,
                    const Position &b, Position pos) {
    return a.getDistance(pos) < a.getDistance(pos);
}


std::string StateCamping::performAggressiveAction() {
    auto shoots = game->getPossibleShoots(tank, true);
    std::cout << "HERE\n";

    for (auto s: shoots) {
        for (auto t: s) {
            std::cout << *t << " ";
        }
        std::cout << "\n";
    }
    auto shoot = game->selectBestShoot(shoots, tank, true);
    if (!shoot.empty())
        return shootToString(shoot);

    auto action = findSafePosition();
    if (!action.empty())
        return action;

    auto possible_shoots = game->getPossibleShoots(tank, true);
    if (!possible_shoots.empty())
        return shootToString(game->selectBestShoot(possible_shoots, tank, false));

    std::vector<Position> positions;
    for (auto pos: tank->getAchievableHexes(game->map)) {
        if (game->map.getHex(tank->getPosition())->danger[0] < tank->getHealthPoints()) {
            positions.push_back(pos);
        }
    }
    auto spawn_position = tank->getSpawnPosition();
    std::sort(positions.begin(), positions.end(),
              [spawn_position](auto &&PH1, auto &&PH2) {
                  return sortbydistance2(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2),
                                        spawn_position);
              });
    if (!positions.empty())
        return moveToString(positions[0]);

    return "";
}
