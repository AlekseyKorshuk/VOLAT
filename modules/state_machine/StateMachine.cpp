#include "StateMachine.h"

StateMachine::StateMachine(Tank* tank, Map* map)  {
    this->tank = tank;
    this->map = map;
};

void StateMachine::changeState(State* new_state) {
    //if (new_state->getType() != state->getType()) {
    state = new_state;
    //}
}

std::string StateMachine::calculateAction()
{
    return state->calculateAction();
}
