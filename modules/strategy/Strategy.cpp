#include "Strategy.h"
#include "../content/vehicles/Tank.h"
#include "../state_machine/MediumTankStrategy.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <nlohmann/json.hpp>
#include <memory>



using json = nlohmann::json;

Strategy::Strategy(json map_json) {
    map = Map(map_json);

    for (auto it: map.player_vehicles) {
        std::shared_ptr<Tank> c = std::static_pointer_cast<Tank>(it->content);
        c->current_strategy_ = new MediumTankStrategy(c.get(), &map);
    }
}

json Strategy::calculate_actions(json state) {
    // TODO calculate actions

    std::string actions;



    map.setMap(state);


    for (auto it: map.player_vehicles) {
        std::shared_ptr<Tank> c = std::static_pointer_cast<Tank>(it->content);
        c->current_strategy_ = new MediumTankStrategy(c.get(), &map);
    }


    for (auto it: map.player_vehicles) {
        std::shared_ptr<Tank> c = std::static_pointer_cast<Tank>(it->content);
        c->current_strategy_ = new MediumTankStrategy(c.get(), &map);
    }


    for (auto it: map.player_vehicles) {
        std::shared_ptr<Tank> c = std::static_pointer_cast<Tank>(it->content);
        c->current_strategy_->updateState();
    }


    int k = 0;
    for (auto it: map.player_vehicles) {
        std::shared_ptr<Tank> c = std::static_pointer_cast<Tank>(it->content);
        std::string action = c->current_strategy_->calculateAction();
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