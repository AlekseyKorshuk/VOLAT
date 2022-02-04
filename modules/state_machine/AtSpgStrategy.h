#pragma once
#include "../state/StateCapture.h"
#include "StateMachine.h"

class AtSpgStrategy : public StateMachine{
public:
    virtual void updateState();
    AtSpgStrategy(std::shared_ptr<Tank>, Game*);
};


