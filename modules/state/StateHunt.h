#pragma once

#include "State.h"
#include "../map/Map.h"
#include "../content/vehicles/Tank.h"
#include "../map/Hex.h"


class StateHunt : public State {
public:
    StateHunt(std::shared_ptr<Tank>, std::shared_ptr<Game>, std::shared_ptr<Param> = std::make_shared<Param>());

    virtual std::string getType();

    virtual std::string calculateAction();
};

