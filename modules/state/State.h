
#pragma once


#include <iostream>
#include <string>
#include "../map/Map.h"
#include "../content/vehicles/Tank.h"

class Tank;
class Game;

class State {
public:
    State(std::shared_ptr<Tank>, Game*);
    virtual std::string getType() = 0;
    virtual std::string calculateAction() = 0;

protected:
    std::shared_ptr<Tank> tank;
    Game* game;


    std::string moveToString(Hex*);
    std::string shootToString(std::vector<std::shared_ptr<Tank>>);
};


