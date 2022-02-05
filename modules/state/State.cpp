#include "State.h"

State::State(std::shared_ptr<Tank> tank, Game *game, Param *param): tank(tank), game(game), param(param){
};

std::string State::moveToString(Hex* hex) {
    game->updateTank(tank->id, hex->x, hex->y, hex->z);

    return "{\"type\":\"MOVE\",\"data\":{\"vehicle_id\":"+ std::to_string(tank->id) +
           ",\"target\":{\"x\":"+std::to_string(hex->x)+",\"y\":"+std::to_string(hex->y)+",\"z\":"+std::to_string(hex->z)+"}}}";
}
std::string State::shootToString(std::vector<std::shared_ptr<Tank>> tanks) {
    for (auto tank: tanks) {
        tank->update(tank->getHealthPoints()-1);
    }

    Hex hex = tanks[0]->getPosition();

    return "{\"type\":\"SHOOT\",\"data\":{\"vehicle_id\":"+ std::to_string(tank->id) +
           ",\"target\":{\"x\":"+std::to_string(hex.x)+",\"y\":"+std::to_string(hex.y)+",\"z\":"+std::to_string(hex.z)+"}}}";
}

