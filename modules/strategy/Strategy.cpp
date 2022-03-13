#include "Strategy.h"
#include "../state_machine/MediumTankStrategy.h"
#include "../state_machine/SpgStrategy.h"
#include "../state_machine/LightTankStrategy.h"
#include "../state_machine/HeavyTankStrategy.h"
#include "../state_machine/AtSpgStrategy.h"
#include <iostream>
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

json Strategy::calculate_actions_old(int idx, json state) {
    std::string actions;

    auto total_start = std::chrono::steady_clock::now();

    auto start = std::chrono::steady_clock::now();
    game->update(state);
    game->predictingBehaviorOpponentsTanks();

    std::cout << "update: " << since(start).count() << " ms" << std::endl;

    for (auto tank: game->player_vehicles)
        tank->current_strategy_->updateState();

    std::vector<std::string> actionList(5);
    std::vector<bool> fCalcAction(5);
    std::vector<std::pair<int, int> > calcAction;

    for (int i = 0; i < game->player_vehicles.size(); i++) {
        calcAction.push_back({game->player_vehicles[i]->current_strategy_->getPriority(), i});
    }

    sort(calcAction.begin(), calcAction.end(),
         [](std::pair<int, int> a, std::pair<int, int> b) {
             return (a.first > b.first) || (a.first == b.first && a.second < b.second);
         });

    int numCalcAction = 0;
    int numDoAction = 0;

    while (numCalcAction != 5 || numDoAction != 5) {
        if (numDoAction != 5 && fCalcAction[numDoAction] == true) {
            auto start = std::chrono::steady_clock::now();

            std::string action = actionList[numDoAction];
            if (!action.empty()) {
                if (!actions.empty()) {
                    actions += ',';
                }
                actions += "\"" + std::to_string(numDoAction) + "\":" + action;
            }

            std::cout << game->player_vehicles[numDoAction]->getStringTankType()
                      << " do " << game->player_vehicles[numDoAction]->current_strategy_->getStateName() << ": ";
            game->player_vehicles[numDoAction]->current_strategy_->doAction(action);
            std::cout << since(start).count() << " ms" << std::endl;

            numDoAction++;
        } else if (numCalcAction != 5) {
            auto start = std::chrono::steady_clock::now();
            int j = calcAction[numCalcAction++].second;
            fCalcAction[j] = true;
            game->player_vehicles[j]->current_strategy_->updateState();
            std::cout << game->player_vehicles[j]->getStringTankType()
                      << " calc " << game->player_vehicles[j]->current_strategy_->getStateName() << ": ";
            actionList[j] = game->player_vehicles[j]->current_strategy_->calculateAction();
            std::cout << since(start).count() << " ms" << std::endl;
        }
    }

    std::cout << "Total elapsed: " << since(total_start).count() << " ms" << std::endl;

    actions = "{" + actions + "}";
    return json::parse(actions);
}

json Strategy::calculate_actions(int idx, json state) {
    json actions;

    auto total_start = std::chrono::steady_clock::now();

    auto start = std::chrono::steady_clock::now();
    game->update(state);
    game->predictingBehaviorOpponentsTanks();

    std::cout << "update: " << since(start).count() << " ms" << std::endl;

    for (auto tank: game->player_vehicles)
        tank->current_strategy_->updateState();

    std::shared_ptr<Tank> medium_tank, light_tank, heavy_tank, spg_tank, at_spg_tank;
    for (const auto &tank: game->player_vehicles) {
        switch (tank->getTankType()) {
            case TankType::MEDIUM:
                medium_tank = tank;
                break;
            case TankType::LIGHT:
                light_tank = tank;
                break;
            case TankType::HEAVY:
                heavy_tank = tank;
                break;
            case TankType::SPG:
                spg_tank = tank;
                break;
            case TankType::AT_SPG:
                at_spg_tank = tank;
                break;
            default:
                break;
        }
    }

    start = std::chrono::steady_clock::now();
    auto first_part = game->calcLightMedium(light_tank, medium_tank);
    std::cout << "Light & Medium elapsed: " << since(start).count() << " ms" << std::endl;

    start = std::chrono::steady_clock::now();
    auto second_part = game->calcSPG_Heavy_At(spg_tank, heavy_tank, at_spg_tank);
    std::cout << "SPG & Heavy & AtSPG elapsed: " << since(start).count() << " ms" << std::endl;

    for (auto data: {first_part, second_part})
        for (json::iterator it = data.begin(); it != data.end(); ++it) {
            std::string action = it.value().get<std::string>();
            if (!action.empty()) {
                actions[it.key()] = json::parse(action);
            }
        }
    int counter = 0;
    json ordered_action = {};
    if (actions.contains(spg_tank->getStringTankType())) {
        ordered_action[counter] = actions[spg_tank->getStringTankType()];
        counter++;
    }
    if (actions.contains(light_tank->getStringTankType())) {
        ordered_action[counter] = actions[light_tank->getStringTankType()];
        counter++;
    }
    if (actions.contains(heavy_tank->getStringTankType())) {
        ordered_action[counter] = actions[heavy_tank->getStringTankType()];
        counter++;
    }
    if (actions.contains(medium_tank->getStringTankType())) {
        ordered_action[counter] = actions[medium_tank->getStringTankType()];
        counter++;
    }
    if (actions.contains(at_spg_tank->getStringTankType())) {
        ordered_action[counter] = actions[at_spg_tank->getStringTankType()];
        counter++;
    }

    std::cout << "Total elapsed: " << since(total_start).count() << " ms" << std::endl;

    return ordered_action;
}