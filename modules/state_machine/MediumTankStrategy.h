#pragma once


#include "../state/StateCapture.h"
#include "StateMachine.h"

class MediumTankStrategy : public StateMachine{
public:
    virtual void updateState();
    MediumTankStrategy(Tank*, Game*);
};


