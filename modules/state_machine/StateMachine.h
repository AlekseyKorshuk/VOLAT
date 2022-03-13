#pragma once

#include "../map/Map.h"
#include "../content/vehicles/Tank.h"
#include "../state/State.h"
#include "../game/Game.h"


class Tank;

class State;

class StateMachine {
public:
    StateMachine(std::shared_ptr<Tank>, std::shared_ptr<Game>);

    virtual void updateState() = 0;

    std::string calculateAction();

    void changeState(std::shared_ptr<State>);

    std::string getStateName();

    std::shared_ptr<State> getState();

    int getPriority();

    void doAction(std::string);

protected:
    std::shared_ptr<State> state = nullptr;
    std::shared_ptr<Game> game = nullptr;
    std::shared_ptr<Tank> tank = nullptr;
};

