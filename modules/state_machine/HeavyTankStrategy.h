#pragma once

#include "StateMachine.h"

/**
 * Heavy Strategy class
 */
class HeavyTankStrategy : public StateMachine {
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
    HeavyTankStrategy(std::shared_ptr<Tank> tank, std::shared_ptr<Game> game);
};


