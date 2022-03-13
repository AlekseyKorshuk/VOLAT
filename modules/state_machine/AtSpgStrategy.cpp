#include "AtSpgStrategy.h"
#include "../state/StateDefence.h"
#include "../state/StateCamping.h"

AtSpgStrategy::AtSpgStrategy(std::shared_ptr<Tank> tank, std::shared_ptr<Game> game) : StateMachine(tank, game) {
}

void AtSpgStrategy::updateState() {

    if (game->isDefenceNeeded(tank))
        changeState(std::make_shared<StateDefence>(tank, game, std::make_shared<Param>()));
    else
        changeState(std::make_shared<StateCamping>(tank, game, std::make_shared<Param>()));
    state->setPriority(105);


}
