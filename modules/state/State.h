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

    Param(std::shared_ptr<Tank> tank) : tank(tank) {}

    ~Param() {}

    Position pos;

    std::shared_ptr<Tank> tank;
    int idp;
};


class State {
public:
    State(std::shared_ptr<Tank>, std::shared_ptr<Game>, std::shared_ptr<Param> = std::make_shared<Param>());

    virtual std::string getType() = 0;

    virtual std::string calculateAction() = 0;

    void doAction(std::string);

    std::string moveToString(Position);

    std::string shootToString(std::vector<std::shared_ptr<Tank>>);

    std::shared_ptr<Param> param;

    int getPriority();

    void setPriority(int);
protected:
    std::shared_ptr<Tank> tank;
    std::shared_ptr<Game> game;
    int priority_ = 0;

};


