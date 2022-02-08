#pragma once

#include "../state/StateCapture.h"
#include "../state/StateDefence.h"

#include "StateMachine.h"

class HeavyTankStrategy : public StateMachine {
public:
    virtual void updateState();

    HeavyTankStrategy(std::shared_ptr<Tank>, std::shared_ptr<Game>);
};


