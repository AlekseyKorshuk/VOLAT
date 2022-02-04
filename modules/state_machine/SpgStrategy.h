#pragma once
#include "../state/StateCapture.h"
#include "StateMachine.h"

class SpgStrategy : public StateMachine{
public:
    virtual void updateState();
    SpgStrategy(std::shared_ptr<Tank>, Game*);
};


