#include "StateCapture.h"

StateCapture::StateCapture(std::shared_ptr<Tank> tank, Game* game) : State(tank, game){

}


std::string StateCapture::getType()  {
    return "capture";
}

std::string StateCapture::calculateAction()
{

    Hex* h = game->map.getHex(tank->getPosition());

    std::vector<Hex*> path = game->map.findPath(h,game->map.base, tank);

    if (path.empty()) return "";

    return moveToString(path[1]);
}
