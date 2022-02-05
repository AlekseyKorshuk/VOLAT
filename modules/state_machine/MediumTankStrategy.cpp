#include "MediumTankStrategy.h"
#include "../state/StateHunt.h"

MediumTankStrategy::MediumTankStrategy(std::shared_ptr<Tank> tank, std::shared_ptr<Game> game) : StateMachine(tank,
                                                                                                              game) {
}

void MediumTankStrategy::updateState() {


    if (state == nullptr || state->getType() != "hunt") {
        for (auto tank_opponent: game->opponent_vehicles) {
            if (!game->findSafePositionsToShoot(tank, tank_opponent).empty()) {
                changeState(std::make_shared<StateHunt>(tank, game, std::make_shared<Param>(tank_opponent)));
                return;
            }
        }
    } else {
        return;
    }

    changeState(std::make_shared<StateCapture>(tank, game, std::make_shared<Param>()));
}
