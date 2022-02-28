#pragma once

#include "StateMachine.h"

class MediumTankStrategy : public StateMachine {
public:
    virtual void updateState();

    MediumTankStrategy(std::shared_ptr<Tank>, std::shared_ptr<Game>);

private:
    const int radius_of_danger = 3;
};



