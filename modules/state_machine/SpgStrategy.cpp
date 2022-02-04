#include "SpgStrategy.h"

SpgStrategy::SpgStrategy(std::shared_ptr<Tank> tank, Game* game) : StateMachine(tank, game) {

}

void SpgStrategy::updateState() {
    changeState(new StateCapture(tank, game));
}
