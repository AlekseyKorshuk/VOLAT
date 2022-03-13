#include "StateHunt.h"
#include <vector> //std::vector

StateHunt::StateHunt(std::shared_ptr<Tank> tank, std::shared_ptr<Game> game, std::shared_ptr<Param> param) : State(tank,
                                                                                                                   game,
                                                                                                                   param) {
}


std::string StateHunt::getType() {
    return "hunt";
}


std::string StateHunt::calculateAction() {
    auto positions_to_shoot = game->findSafePositionsToShoot(tank, param->tank, true);
    Position start = tank->getPosition();
    if (!positions_to_shoot.empty())
        for (auto tankList: game->tanksUnderShoot(tank))
            for (auto tank_under_shoot: tankList)
                if (tank_under_shoot->getPosition() == param->tank->getPosition())
                    return shootToString(tankList);

    auto path = game->findSafePath(start, positions_to_shoot, tank);
    if (path.size() != 0)
        return moveToString(path[1]);
    return "";
}
