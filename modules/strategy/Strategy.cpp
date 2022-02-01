#include "Strategy.h"
#include "../content/vehicles/Tank.h"
#include "../state_machine/MediumTankStrategy.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <nlohmann/json.hpp>
#include <memory>



using json = nlohmann::json;

Strategy::Strategy(int idx, json map_json, json state_json){
    game = Game(idx, map_json, state_json);




    for (auto tank: game.player_vehicles) {
        if (tank != nullptr) {
            switch(tank->getTankType()) {
                case TankType::MEDIUM:
                    tank->current_strategy_ = new MediumTankStrategy(tank, &game);
                    break;
            }
        } else {
            tank->current_strategy_ = nullptr;
        }
    }
}

json Strategy::calculate_actions(int idx, json state) {
    // TODO calculate actions

    std::string actions;

    game.update(state);

    for (auto tank: game.player_vehicles)  if (tank->current_strategy_  != nullptr) {
        tank->current_strategy_->updateState();
    }

    int k = 0;
    for (auto tank: game.player_vehicles)  if (tank->current_strategy_  != nullptr)  {

        std::string action = tank->current_strategy_->calculateAction();
        if (!action.empty()) {
            if (!actions.empty()) {
                actions += ',';
            }

            actions += "\"" + std::to_string(k++) + "\":" + action;

        }
    }

    actions = "{" + actions + "}";

    return json::parse(actions);
}