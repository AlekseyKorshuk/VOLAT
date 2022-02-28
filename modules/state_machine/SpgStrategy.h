#pragma once

#include "StateMachine.h"

class SpgStrategy : public StateMachine {
public:
    virtual void updateState();

    SpgStrategy(std::shared_ptr<Tank>, std::shared_ptr<Game>);
};


