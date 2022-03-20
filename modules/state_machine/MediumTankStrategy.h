#pragma once

#include "StateMachine.h"

/**
 * Medium Strategy class
 */
class MediumTankStrategy : public StateMachine {
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
    MediumTankStrategy(std::shared_ptr<Tank> tank, std::shared_ptr<Game> game);

private:
    const int radius_of_danger = 3;
};



