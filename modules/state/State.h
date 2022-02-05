#pragma once


#include <iostream>
#include <string>
#include "../map/Map.h"
#include "../content/vehicles/Tank.h"

class Tank;
class Game;

union Param {
    Param() {
        idp = -1;
    }
    ~Param() {}

    std::shared_ptr<Tank> tank;
    int idp;
};


class State {
public:
    State(std::shared_ptr<Tank>, Game*, Param* = new Param);
    virtual std::string getType() = 0;
    virtual std::string calculateAction() = 0;

    Param* param;
protected:
    std::shared_ptr<Tank> tank;
    Game* game;


    std::string moveToString(Hex*);
    std::string shootToString(std::vector<std::shared_ptr<Tank>>);
};


