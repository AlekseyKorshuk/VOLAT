
#pragma once


#include <iostream>
#include <string>
#include "../map/Map.h"
#include "../content/vehicles/Tank.h"

class Tank;

class State {
public:
    State(Tank*, Map*);
    virtual std::string getType() = 0;
    virtual std::string calculateAction() = 0;

protected:
    Tank* tank;
    Map* map;


    std::string moveToString(Hex*);
    std::string shootToString(Hex*);
};


