#pragma once

#include "../state/StateCapture.h"
#include "StateMachine.h"

class MediumTankStrategy : public StateMachine {
public:
    virtual void updateState();

    MediumTankStrategy(std::shared_ptr<Tank>, std::shared_ptr<Game>);
};


