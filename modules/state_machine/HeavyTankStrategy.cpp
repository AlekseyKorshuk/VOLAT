#include "HeavyTankStrategy.h"
#include "../state/StateCapture.h"
#include "../state/StateHealth.h"

HeavyTankStrategy::HeavyTankStrategy(std::shared_ptr<Tank> tank, std::shared_ptr<Game> game) : StateMachine(tank,
                                                                                                            game) {
}

void HeavyTankStrategy::updateState() {

    changeState(std::make_shared<StateCapture>(tank, game, std::make_shared<Param>()));
    state->setPriority(100);

    return;

    if (state != nullptr) {
        if (state->getType() == "health" && tank->getHealthPoints() != tank->getDestructionPoints()) return;
    }

    if (game->isHealthNeeded(tank))
        changeState(std::make_shared<StateHealth>(tank, game, std::make_shared<Param>()));
    else
        changeState(std::make_shared<StateCapture>(tank, game, std::make_shared<Param>()));
}
