#include "Core.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <nlohmann/json.hpp>
#include <chrono>

using json = nlohmann::json;

#include "../client/Client.h"
#include "../strategy/Strategy.h"

template<
        class result_t   = std::chrono::milliseconds,
        class clock_t    = std::chrono::steady_clock,
        class duration_t = std::chrono::milliseconds
>
auto since(std::chrono::time_point<clock_t, duration_t> const &start) {
    return std::chrono::duration_cast<result_t>(clock_t::now() - start);
}

Core::Core(string name, string password) {
    this->name = name;
    this->password = password;
}

void Core::play(string game, int num_turns, int num_players) {

    Client client = Client();
    response resp = client.login(this->name, this->password, game, num_turns, num_players);
    int idx = resp.msg["idx"].get<std::int32_t>();
    int idr = idx;

    json map_json = client.map().msg;
    json state_json = client.game_state().msg;
    Strategy strategy(idx, map_json, state_json);

    while (true) {
        json state = client.game_state().msg;


        if (state["finished"]) {
            std::cout << "Game is finished" << std::endl;
            break;
        }
//        else{
//            std::cout << state["current_turn"] << endl;
//            std::cout << state["win_points"] << endl;
//        }


        if (idx == state["current_player_idx"].get<std::int32_t>()) {
            cout << "Our turn!" << endl;
            auto start = std::chrono::steady_clock::now();

            json actions = strategy.calculate_actions(idx, state);

            for (json::iterator it = actions.begin(); it != actions.end(); ++it) {
                json action = it.value();
                json data = action["data"];
                string action_type = action["type"].get<std::string>();
                int vehicle_id = data["vehicle_id"].get<std::int32_t>();
                int x = data["target"]["x"].get<std::int32_t>();
                int y = data["target"]["y"].get<std::int32_t>();
                int z = data["target"]["z"].get<std::int32_t>();

                if (action_type == "MOVE") {
                    std::cout << "MOVE: " << vehicle_id << " {" << x << " " << y << " " << z << "}\n";
                    cout << client.move(vehicle_id, x, y, z).msg << '\n';
                } else if (action_type == "SHOOT") {
                    std::cout << "SHOOT: " << vehicle_id << " {" << x << " " << y << " " << z << "}\n";
                    cout << client.shoot(vehicle_id, x, y, z).msg << '\n';
                }
            }
            std::cout << "Elapsed: " << since(start).count() << " ms" << std::endl;
            client.turn();
        } else
            if (idr != state["current_player_idx"].get<std::int32_t>()) {
                client.turn();
            }

        idr = state["current_player_idx"].get<std::int32_t>();
    }
}