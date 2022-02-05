#include "LightTankStrategy.h"

LightTankStrategy::LightTankStrategy(std::shared_ptr<Tank> tank, std::shared_ptr<Game> game) : StateMachine(tank,
                                                                                                            game) {
}

void LightTankStrategy::updateState() {
    changeState(std::make_shared<StateCapture>(tank, game, std::make_shared<Param>()));
}
