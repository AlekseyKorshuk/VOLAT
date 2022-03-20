#pragma once

#include "State.h"
#include "../map/Map.h"
#include "../content/vehicles/Tank.h"
#include "../map/Hex.h"

/**
 * StateHunt class
 */
class StateHunt : public State {
public:
    /**
     * Constructor
     * @param tank Tank
     * @param game Game
     * @param param Param
     */
    StateHunt(std::shared_ptr<Tank> tank, std::shared_ptr<Game> game,
              std::shared_ptr<Param> param = std::make_shared<Param>());

    virtual std::string getType();

    virtual std::string calculateAction();
};


