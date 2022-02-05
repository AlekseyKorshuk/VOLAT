#include "StateDefence.h"

StateDefence::StateDefence(std::shared_ptr<Tank> tank, std::shared_ptr<Game> game, std::shared_ptr<Param> param)
        : State(tank, game,
                param) {
}


std::string StateDefence::getType() {
    return "defence";
}


#include <algorithm> //std::sort
#include <iostream> //std::cout
#include <string> //std::string
#include <vector> //std::vector


std::string StateDefence::calculateAction() {
    std::shared_ptr<Hex> position = game->map.getHex(tank->getPosition());

    return "";
}
