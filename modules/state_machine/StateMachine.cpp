#include "StateMachine.h"

StateMachine::StateMachine(std::shared_ptr<Tank> tank, Game* game)  {
    this->tank = tank;
    this->game = game;
};

void StateMachine::changeState(State* new_state) {
    if (state == nullptr || new_state->getType() != state->getType()) {
        state = new_state;
    }

}

std::string StateMachine::calculateAction()
{
    return state->calculateAction();
}
