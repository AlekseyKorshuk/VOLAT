#pragma once
#include "State.h"
#include "../map/Map.h"
#include "../content/vehicles/Tank.h"
#include "../map/Hex.h"

class StateCapture : public State{
public:
    StateCapture(Tank*, Map*);
    virtual std::string getType();
    virtual std::string calculateAction();
};


