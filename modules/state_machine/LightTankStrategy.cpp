#include "LightTankStrategy.h"

LightTankStrategy::LightTankStrategy(std::shared_ptr<Tank> tank, Game* game) : StateMachine(tank, game) {

}

void LightTankStrategy::updateState() {
    changeState(new StateCapture(tank, game));
}
