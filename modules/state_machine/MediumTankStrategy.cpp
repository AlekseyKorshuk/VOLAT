#include "MediumTankStrategy.h"
#include "../state/StateHunt.h"

MediumTankStrategy::MediumTankStrategy(std::shared_ptr<Tank> tank, Game* game) : StateMachine(tank, game) {
}

void MediumTankStrategy::updateState() {


    if (state == nullptr || state->getType() != "hunt") {
        for (auto tank_opponent: game->opponent_vehicles)
        {
            if(!game->findSafePositionsToShoot(tank, tank_opponent).empty()) {
                changeState(new StateHunt(tank, game, new Param(tank_opponent)));
                return;
            }
        }
    } else {
        return;
    }

    changeState(new StateCapture(tank, game, new Param()));
}
