#include "MediumTankStrategy.h"

MediumTankStrategy::MediumTankStrategy(Tank* tank, Game* game) : StateMachine(tank, game) {

}

void MediumTankStrategy::updateState() {
    changeState(new StateCapture(tank, game));
}
