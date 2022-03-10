#include "StateMachine.h"

StateMachine::StateMachine(std::shared_ptr<Tank> tank, std::shared_ptr<Game> game) {
    this->tank = tank;
    this->game = game;
};

void StateMachine::changeState(std::shared_ptr<State> new_state) {
    if (state == nullptr || new_state->getType() != state->getType()) {
        state = new_state;
    }
}

std::string StateMachine::calculateAction() {
    return state->calculateAction();
}

std::string StateMachine::getStateName() {
    return state->getType();
}

std::shared_ptr<State> StateMachine::getState() {
    return state;
}

int StateMachine::getPriority() {
    return state->getPriority();
}

void StateMachine::doAction(std::string s) {
    state->doAction(s);
}