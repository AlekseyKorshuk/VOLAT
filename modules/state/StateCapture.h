#pragma once
#include "State.h"
#include "../map/Map.h"
#include "../content/vehicles/Tank.h"
#include "../map/Hex.h"

class StateCapture : public State{
public:
    StateCapture(std::shared_ptr<Tank>, Game*);
    virtual std::string getType();
    virtual std::string calculateAction();
};


