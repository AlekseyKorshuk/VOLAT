#include "Strategy.h"
#include "../content/vehicles/Tank.h"
#include "../state_machine/MediumTankStrategy.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <nlohmann/json.hpp>
#include <memory>



using json = nlohmann::json;

Strategy::Strategy(json map_json, json state_json) {
    game = Game(map_json, state_json);



    for (auto tank: game.player_vehicles) {
        tank->current_strategy_ = new MediumTankStrategy(tank.get(), &game);
    }
}

json Strategy::calculate_actions(json state) {
    // TODO calculate actions

    std::string actions;

    game.update(state);

    for (auto tank: game.player_vehicles) {
        tank->current_strategy_->updateState();
    }

    int k = 0;
    for (auto tank: game.player_vehicles) {

        std::string action = tank->current_strategy_->calculateAction();
        if (!action.empty()) {
            if (!actions.empty()) {
                actions += ',';
            }

            actions += "\"" + std::to_string(k++) + "\":" + action;

        }
    }


    /*
    std::string actions = "{\"0\":{\"type\":\"MOVE\",\"data\":{\"vehicle_id\":5,\"target\":{\"x\":-1,\"y\":1,\"z\":0}}},"
                          "\"1\":{\"type\":\"SHOOT\",\"data\":{\"vehicle_id\":5,\"target\":{\"x\":-1,\"y\":1,\"z\":0}}}}";
     */
    actions = "{" + actions + "}";

    return json::parse(actions);
}