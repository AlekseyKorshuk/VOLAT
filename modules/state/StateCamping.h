#pragma once

#include "State.h"
#include "../map/Map.h"
#include "../content/vehicles/Tank.h"
#include "../map/Hex.h"


class StateCamping : public State {
public:
    /**
     * Constructor
     * @param tank Tank
     * @param game Game
     * @param param Param
     */
    StateCamping(std::shared_ptr<Tank> tank, std::shared_ptr<Game> game,
                 std::shared_ptr<Param> param = std::make_shared<Param>());

    virtual std::string getType();

    virtual std::string calculateAction();

    /**
     * Method to find safe position
     * @return Resulting action
     */
    virtual std::string findSafePosition();

    /**
     * Method to find perform aggressive action
     * @return Resulting action
     */
    virtual std::string performAggressiveAction();
};


