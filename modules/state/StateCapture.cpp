#include "StateCapture.h"

StateCapture::StateCapture(Tank* tank, Map* map) : State(tank, map){

}


std::string StateCapture::getType()  {
    return "capture";
}

std::string StateCapture::calculateAction()
{

    Hex* h = map->getHex(tank->getPosition());

    std::vector<Hex*> path = map->findPath(h,map->base);

    if (path.empty()) return "";

    return moveToString(path[1]);
}
