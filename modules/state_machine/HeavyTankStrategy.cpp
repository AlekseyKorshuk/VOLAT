#include "HeavyTankStrategy.h"

HeavyTankStrategy::HeavyTankStrategy(std::shared_ptr<Tank> tank, Game* game) : StateMachine(tank, game) {
}

void HeavyTankStrategy::updateState() {
    changeState(new StateCapture(tank, game, new Param));
}
