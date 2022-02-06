#include "AtSpgStrategy.h"

AtSpgStrategy::AtSpgStrategy(std::shared_ptr<Tank> tank, std::shared_ptr<Game> game) : StateMachine(tank, game) {
}

void AtSpgStrategy::updateState() {
    changeState(std::make_shared<StateCamping>(tank, game, std::make_shared<Param>()));
}
