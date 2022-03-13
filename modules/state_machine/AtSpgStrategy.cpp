#include "AtSpgStrategy.h"
#include "../state/StateDefence.h"
#include "../state/StateCamping.h"
#include "../state/StateCapture.h"
#include "../state/StateHealth.h"

AtSpgStrategy::AtSpgStrategy(std::shared_ptr<Tank> tank, std::shared_ptr<Game> game) : StateMachine(tank, game) {
}

void AtSpgStrategy::updateState() {
    //changeState(std::make_shared<StateCapture>(tank, game, std::make_shared<Param>()));
    //return;
    if (state != nullptr) {
        if (state->getType() == "health" && tank->getHealthPoints() != tank->getDestructionPoints()) return;
    }

    if (game->isDefenceNeeded(tank))
        changeState(std::make_shared<StateDefence>(tank, game, std::make_shared<Param>()));
    if (state != nullptr) {
        if (state->getType() == "health" && tank->getHealthPoints() != tank->getDestructionPoints()) return;
    }
    if (game->isHealthNeeded(tank))
        changeState(std::make_shared<StateHealth>(tank, game, std::make_shared<Param>()));
    else if (game->isCaptureNeeded(tank))
        changeState(std::make_shared<StateCapture>(tank, game, std::make_shared<Param>()));
    else
        changeState(std::make_shared<StateCamping>(tank, game, std::make_shared<Param>()));
    state->setPriority(40);

}
