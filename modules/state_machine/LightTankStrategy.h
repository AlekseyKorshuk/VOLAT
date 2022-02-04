#pragma once
#include "../state/StateCapture.h"
#include "StateMachine.h"

class LightTankStrategy : public StateMachine{
public:
    virtual void updateState();
    LightTankStrategy(std::shared_ptr<Tank>, Game*);
};
