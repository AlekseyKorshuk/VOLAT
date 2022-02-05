#include "SpgStrategy.h"

SpgStrategy::SpgStrategy(std::shared_ptr<Tank> tank, std::shared_ptr<Game> game) : StateMachine(tank, game) {
}

void SpgStrategy::updateState() {
    changeState(std::make_shared<StateCapture>(tank, game, std::make_shared<Param>()));
}
