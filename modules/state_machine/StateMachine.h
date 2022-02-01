#pragma once



#include "../map/Map.h"
#include "../content/vehicles/Tank.h"
#include "../state/State.h"
#include "../game/Game.h"


class Tank;
class State;

class StateMachine {
public:
    StateMachine(Tank*, Game*);

    virtual void updateState() = 0;

    std::string calculateAction();
    void changeState(State*);


protected:
    State* state = nullptr;
    Game* game = nullptr;
    Tank* tank = nullptr;
};

