#include "State.h"

State::State(Tank* tank, Game* game): tank(tank), game(game){

};

std::string State::moveToString(Hex* hex) {


    game->updateTank(tank->id, hex->x, hex->y, hex->z);

    return "{\"type\":\"MOVE\",\"data\":{\"vehicle_id\":"+ std::to_string(tank->id) +
           ",\"target\":{\"x\":"+std::to_string(hex->x)+",\"y\":"+std::to_string(hex->y)+",\"z\":"+std::to_string(hex->z)+"}}}";
}
std::string State::shootToString(Hex* hex) {
    return "{\"type\":\"SHOOT\",\"data\":{\"vehicle_id\":"+ std::to_string(tank->id) +
           ",\"target\":{\"x\":"+std::to_string(hex->x)+",\"y\":"+std::to_string(hex->y)+",\"z\":"+std::to_string(hex->z)+"}}}";
}

