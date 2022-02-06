#include "StateCamping.h"
#include "StateHunt.h"

StateCamping::StateCamping(std::shared_ptr<Tank> tank, std::shared_ptr<Game> game, std::shared_ptr<Param> param) : State(tank,
                                                                                                                   game,
                                                                                                                   param) {
}


std::string StateHunt::getType() {
    return "camping";
}