#include "AtSpgStrategy.h"

AtSpgStrategy::AtSpgStrategy(std::shared_ptr<Tank> tank, Game* game) : StateMachine(tank, game) {
}

void AtSpgStrategy::updateState() {
    changeState(new StateCapture(tank, game));
}
