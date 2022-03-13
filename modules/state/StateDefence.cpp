#include "StateDefence.h"
#include <algorithm> //std::sort
#include <vector> //std::vector


StateDefence::StateDefence(std::shared_ptr<Tank> tank, std::shared_ptr<Game> game, std::shared_ptr<Param> param)
        : State(tank, game,
                param) {
}


std::string StateDefence::getType() {
    return "defence";
}


std::string StateDefence::calculateAction() {
    auto action_save = game->saveTeam(tank);
    if (!action_save.empty())
        return action_save;

    auto position = tank->getPosition();

    auto shoots = game->getPossibleShoots(tank, true);

    auto shoot = game->selectBestShootDefence(shoots, tank);
    if (!shoot.empty())
        return shootToString(shoot);

    if (std::find(game->map.base.begin(), game->map.base.end(), position) == game->map.base.end()) {
        // Not on base
        auto path = game->smartFindQuickPath(tank->getPosition(), game->map.base, tank);
        if (!path.empty())
            return moveToString(path[1]);
    }
    auto possible_shoots = game->getPossibleShoots(tank, true);
    if (!possible_shoots.empty())
        return shootToString(game->selectBestShoot(possible_shoots, tank, false));

    auto tanks_on_base = game->findTanksToShootOnArea(game->map.base);
    std::vector<Position> positions_list;
    for (const auto &opponent_tank: tanks_on_base) {
        auto positions = game->findSafePositionsToShoot(tank, opponent_tank, false);
        positions_list.insert(positions_list.end(), positions.begin(), positions.end());
    }
    auto path = game->smartFindQuickPath(tank->getPosition(), positions_list, tank);
    if (!path.empty())
        return moveToString(path[1]);
    return "";
}
