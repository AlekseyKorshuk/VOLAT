#pragma once

#include "StateMachine.h"

class AtSpgStrategy : public StateMachine {
public:
    virtual void updateState();

    AtSpgStrategy(std::shared_ptr<Tank>, std::shared_ptr<Game>);
};


