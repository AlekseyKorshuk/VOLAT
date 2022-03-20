#pragma once

#include "StateMachine.h"

/**
 * AT-SPG Strategy class
 */
class AtSpgStrategy : public StateMachine {
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
    AtSpgStrategy(std::shared_ptr<Tank> tank, std::shared_ptr<Game> game);
};


