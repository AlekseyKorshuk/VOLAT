#include "MediumTankStrategy.h"
#include "../state/StateCapture.h"
#include "../state/StateDefence.h"
#include "../state/StateCamping.h"
#include "../state/StateHealth.h"

MediumTankStrategy::MediumTankStrategy(std::shared_ptr<Tank> tank, std::shared_ptr<Game> game) : StateMachine(tank,
                                                                                                              game) {
}

void MediumTankStrategy::updateState() {

    if (game->isDefenceNeeded(tank))
        changeState(std::make_shared<StateDefence>(tank, game, std::make_shared<Param>()));
    else if (game->isCaptureNeeded(tank))
        changeState(std::make_shared<StateCapture>(tank, game, std::make_shared<Param>()));
    else
        changeState(std::make_shared<StateCamping>(tank, game, std::make_shared<Param>()));
    state->setPriority(80);
}
