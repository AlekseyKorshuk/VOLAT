#pragma once
#include "../state/StateCapture.h"
#include "StateMachine.h"

class HeavyTankStrategy : public StateMachine{
public:
    virtual void updateState();
    HeavyTankStrategy(std::shared_ptr<Tank>, Game*);
};

