#pragma once

#include "StateMachine.h"

class LightTankStrategy : public StateMachine {
public:
    virtual void updateState();

    LightTankStrategy(std::shared_ptr<Tank>, std::shared_ptr<Game>);

private:
    const int radius_of_danger = 4;
};
