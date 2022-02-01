#include "MediumTankStrategy.h"

MediumTankStrategy::MediumTankStrategy(std::shared_ptr<Tank> tank, Game* game) : StateMachine(tank, game) {

}

void MediumTankStrategy::updateState() {
    changeState(new StateCapture(tank, game));
}
