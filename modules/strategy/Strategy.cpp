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
#include <chrono>


using json = nlohmann::json;

template<
        class result_t   = std::chrono::milliseconds,
        class clock_t    = std::chrono::steady_clock,
        class duration_t = std::chrono::milliseconds
>
auto since(std::chrono::time_point<clock_t, duration_t> const &start) {
    return std::chrono::duration_cast<result_t>(clock_t::now() - start);
}


Strategy::Strategy(int idx, json map_json, json state_json) {
    game = std::make_shared<Game>(idx, map_json, state_json);

    std::sort(game->player_vehicles.begin(), game->player_vehicles.end(),
              [](std::shared_ptr<Tank> a, std::shared_ptr<Tank> b) {
                  return int(a->getTankType()) < int(b->getTankType());
              }
    );

    for (auto tank: game->player_vehicles) {
        if (tank != nullptr) {
            tank->current_strategy_ = nullptr;
            /*
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
            */
        } else {
            tank->current_strategy_ = nullptr;
        }
    }
}

json Strategy::calculate_actions(int idx, json state) {
    std::string actions;



    auto start = std::chrono::steady_clock::now();
    game->update(state);
    std::cout << "update: " << since(start).count() << " ms" << std::endl;

    for (auto tank: game->player_vehicles)
        if (tank->current_strategy_ != nullptr) {
            tank->current_strategy_->updateState();
        }

    int k = 0;
    auto total_start = std::chrono::steady_clock::now();
    for (auto tank: game->player_vehicles)
        if (tank->current_strategy_ != nullptr) {
            auto start = std::chrono::steady_clock::now();
            std::string action = tank->current_strategy_->calculateAction();
            std::cout << tank->current_strategy_->getStateName() << ": " << since(start).count() << " ms" << std::endl;

            if (!action.empty()) {
                if (!actions.empty()) {
                    actions += ',';
                }

                actions += "\"" + std::to_string(k++) + "\":" + action;

            }
        }
    std::cout << "Total elapsed: " << since(total_start).count() << " ms" << std::endl;


    actions = "{" + actions + "}";

    return json::parse(actions);
}