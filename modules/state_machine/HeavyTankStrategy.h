#pragma once


#include "StateMachine.h"

class HeavyTankStrategy : public StateMachine {
public:
    virtual void updateState();

    HeavyTankStrategy(std::shared_ptr<Tank>, std::shared_ptr<Game>);
};


