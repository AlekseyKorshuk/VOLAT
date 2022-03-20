#pragma once

#include "StateMachine.h"

/**
 * Light Strategy class
 */
class LightTankStrategy : public StateMachine {
public:
    /**
     * Method that updates tank state
     */
    virtual void updateState();

    /**
     * Constructor
     * @param tank Tank
     * @param game Game
     */
    LightTankStrategy(std::shared_ptr<Tank> tank, std::shared_ptr<Game> game);

private:
    const int radius_of_danger = 4;
};
