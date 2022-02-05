#include "Strategy.h"
#include "../content/vehicles/Tank.h"
#include "../state_machine/MediumTankStrategy.h"
#include "../state_machine/SpgStrategy.h"
#include "../state_machine/LightTankStrategy.h"
#include "../state_machine/HeavyTankStrategy.h"
#include "../state_machine/AtSpgStrategy.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <nlohmann/json.hpp>
#include <memory>


using json = nlohmann::json;

Strategy::Strategy(int idx, json map_json, json state_json) {
    game = std::make_shared<Game>(idx, map_json, state_json);

    std::sort(game->player_vehicles.begin(), game->player_vehicles.end(),
              [](std::shared_ptr<Tank> a, std::shared_ptr<Tank> b) {
                  return int(a->getTankType()) < int(b->getTankType());
              }
    );

    for (auto tank: game->player_vehicles) {
        if (tank != nullptr) {
            switch (tank->getTankType()) {
                case TankType::SPG:
                    tank->current_strategy_ = std::make_shared<SpgStrategy>(tank, game);
                    break;
                case TankType::LIGHT:
                    tank->current_strategy_ = std::make_shared<LightTankStrategy>(tank, game);
                    break;
                case TankType::HEAVY:
                    tank->current_strategy_ = std::make_shared<HeavyTankStrategy>(tank, game);
                    break;
                case TankType::MEDIUM:
                    tank->current_strategy_ = std::make_shared<MediumTankStrategy>(tank, game);
                    break;
                case TankType::AT_SPG:
                    tank->current_strategy_ = std::make_shared<AtSpgStrategy>(tank, game);
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

    game->update(state);

    for (auto tank: game->player_vehicles)
        if (tank->current_strategy_ != nullptr) {
            tank->current_strategy_->updateState();
        }

    int k = 0;
    for (auto tank: game->player_vehicles)
        if (tank->current_strategy_ != nullptr) {
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