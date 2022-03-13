#include "SpgStrategy.h"
#include "../state/StateDefence.h"
#include "../state/StateCamping.h"
#include "../state/StateCapture.h"

SpgStrategy::SpgStrategy(std::shared_ptr<Tank> tank, std::shared_ptr<Game> game) : StateMachine(tank, game) {
}

void SpgStrategy::updateState() {
    if (game->isDefenceNeeded(tank))
        changeState(std::make_shared<StateDefence>(tank, game, std::make_shared<Param>()));
    else
        changeState(std::make_shared<StateCamping>(tank, game, std::make_shared<Param>()));
    state->setPriority(60);
}
