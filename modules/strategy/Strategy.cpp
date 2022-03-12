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
    std::string actions;

    auto total_start = std::chrono::steady_clock::now();

    auto start = std::chrono::steady_clock::now();
    game->update(state);
    game->predictingBehaviorOpponentsTanks();

    std::cout << "update: " << since(start).count() << " ms" << std::endl;

    for (auto tank: game->player_vehicles)
        tank->current_strategy_->updateState();

//    std::vector<std::shared_ptr<Tank>> tanks(game->player_vehicles.size());
//    std::copy(game->player_vehicles.begin(), game->player_vehicles.end(), tanks.begin());

//    std::vector<std::pair<int,std::string>> id_action;
//
//    while (!tanks.empty()){
//        double best_score = -99999999;
//        int best_index = 0;
//        std::string best_action;
//        for (int i = 0; i < tanks.size(); i++){
//            game->update(state);
//            for (const auto& it: id_action){
//                std::shared_ptr<Tank> this_tank;
//                for (auto t: game->player_vehicles)
//                    if (t->id == )
//                tanks[i]->current_strategy_->doAction(it.second);
//            }
//            tanks[i]->current_strategy_->updateState();
//            auto action = tanks[i]->current_strategy_->calculateAction();
//            tanks[i]->current_strategy_->doAction(action);
//            auto score = game->calculateCurrentStateScore();
//            if (best_score < score){
//                best_score = score;
//                best_index = i;
//                best_action = action;
//            }
//        }
////        tanks[best_index]->current_strategy_->doAction(best_action);
//        id_action.emplace_back(tanks[best_index]->getTankType(), best_action);
//        tanks.erase (tanks.begin() + best_index);
//    }
//
//    std::sort(id_action.begin(), id_action.end());
//    for(int i = 0; i < id_action.size(); i++){
//        if (!id_action[i].second.empty()) {
//            if (!actions.empty()) {
//                actions += ',';
//            }
//            actions += "\"" + std::to_string(i) + "\":" + id_action[i].second;
//        }
//    }

//    game->update(state);
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