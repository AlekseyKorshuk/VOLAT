#pragma once



#include "../map/Map.h"
#include "../content/vehicles/Tank.h"
#include "../state/State.h"


class Tank;
class State;

class StateMachine {
public:
    StateMachine(Tank*, Map*);

    virtual void updateState() = 0;

    std::string calculateAction();
    void changeState(State*);


protected:
    State* state = nullptr;
    Map* map = nullptr;
    Tank* tank = nullptr;
};

