#pragma once

#include "StateMachine.h"

/**
 * SPG Strategy class
 */
class SpgStrategy : public StateMachine {
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
    SpgStrategy(std::shared_ptr<Tank> tank, std::shared_ptr<Game> game);
};


