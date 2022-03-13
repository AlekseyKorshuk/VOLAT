#include "HeavyTankStrategy.h"
#include "../state/StateCapture.h"

HeavyTankStrategy::HeavyTankStrategy(std::shared_ptr<Tank> tank, std::shared_ptr<Game> game) : StateMachine(tank,
                                                                                                            game) {
}

void HeavyTankStrategy::updateState() {
    changeState(std::make_shared<StateCapture>(tank, game, std::make_shared<Param>()));
    state->setPriority(100);
}
