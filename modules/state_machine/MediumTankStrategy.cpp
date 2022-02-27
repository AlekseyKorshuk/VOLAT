#include "MediumTankStrategy.h"
#include "../state/StateHunt.h"
#include "../state/StateDefence.h"

MediumTankStrategy::MediumTankStrategy(std::shared_ptr<Tank> tank, std::shared_ptr<Game> game) : StateMachine(tank,
                                                                                                              game) {
}

void MediumTankStrategy::updateState() {
    changeState(std::make_shared<StateCapture>(tank, game, std::make_shared<Param>()));
    /*
    //Val1 - tank capture points. Val2 - player capture points. Val3 - player kill points
    std::pair<int,std::pair< int, int> >max_win_point = {-1,{-1, -1}};
    std::shared_ptr<Tank> tank_for_hunting = nullptr;
    Hex center_of_base(0,0,0);

    for (auto opponent_tank: game->opponent_vehicles) {
        if (opponent_tank->getTankType() == TankType::AT_SPG
            || opponent_tank->getTankType() == TankType::SPG) {
            Hex pos = opponent_tank->getPosition();

            if (pos.getDistance(center_of_base) <= radius_of_danger) {
                std::pair<int,std::pair< int, int> > win_point;
                win_point.first = opponent_tank->getCapturePoints();
                win_point.second.first = game->getPlayer(opponent_tank->getPlayerId())->point_capture;
                win_point.second.first = game->getPlayer(opponent_tank->getPlayerId())->point_kill;

                if (win_point > max_win_point) {
                    max_win_point = win_point;
                    tank_for_hunting = opponent_tank;
                }
            }
        }
    }

    if (tank_for_hunting != nullptr) {
        changeState(std::make_shared<StateHunt>(tank, game, std::make_shared<Param>(tank_for_hunting)));
    } else {
        changeState(std::make_shared<StateDefence>(tank, game, std::make_shared<Param>()));
    }
     */
}
