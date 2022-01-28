#include "MediumTankStrategy.h"

MediumTankStrategy::MediumTankStrategy(Tank* tank, Map* map) : StateMachine(tank, map) {

}

void MediumTankStrategy::updateState() {
    changeState(new StateCapture(tank, map));
}
