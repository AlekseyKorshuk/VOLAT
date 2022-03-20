#include "Core.h"
#include <iostream>
#include <cstring>
#include <nlohmann/json.hpp>
#include <thread>


using json = nlohmann::json;

#include "../client/Client.h"
#include "../strategy/Strategy.h"

Core::Core(const std::string &name, const std::string &password) {
    this->name = name;
    this->password = password;
}

void Core::play(std::string game, int num_turns, int num_players) {

    Client client = Client();
    response resp = client.login(this->name, this->password, game, num_turns, num_players);
    std::cout << resp.msg << std::endl;
    int idx = resp.msg["idx"].get<std::int32_t>();
    int idr = idx;

    while (true) {
        json state_json = client.game_state().msg;
        int num_players = state_json["num_players"];
        int num_log_players = 0;

        for (auto player: state_json["players"]) {
            num_log_players++;
        }
        if (num_players == num_log_players) {
            break;
        }
    }

    json state_json = client.game_state().msg;
    json map_json = client.map().msg;
    Strategy strategy(idx, map_json, state_json);


    while (true) {
        json state = client.game_state().msg;

        if (state["finished"]) {
            std::cout << "Game is finished" << std::endl;
            break;
        }

        if (idx == state["current_player_idx"].get<std::int32_t>()) {
            std::cout << "OUR TURN #" << state["current_turn"] << std::endl;
            json actions = strategy.calculate_actions(idx, state);
            for (json::iterator it = actions.begin(); it != actions.end(); ++it) {
                json action = it.value();
                json data = action["data"];
                std::string action_type = action["type"].get<std::string>();
                int vehicle_id = data["vehicle_id"].get<std::int32_t>();
                int x = data["target"]["x"].get<std::int32_t>();
                int y = data["target"]["y"].get<std::int32_t>();
                int z = data["target"]["z"].get<std::int32_t>();
                std::string action_name;
                json msg = "";

                if (action_type == "MOVE") {
                    action_name = "MOVE";
                    msg = client.move(vehicle_id, x, y, z).msg;
                } else if (action_type == "SHOOT") {
                    action_name = "SHOOT";
                    msg = client.shoot(vehicle_id, x, y, z).msg;
                }
                if (!action_name.empty()) {
                    auto tank = strategy.game->getTankByID(vehicle_id);
                    std::cout << "[" << tank->id << "]" << tank->getStringTankType() << " " << action_name << ": "
                              << vehicle_id << " {" << x << " " << y << " " << z << "} -> " << msg << "\n";
                }

            }
            client.turn();
        } else if (idr != state["current_player_idx"].get<std::int32_t>()) {
            client.turn();
        }

        idr = state["current_player_idx"].get<std::int32_t>();
    }
    client.logout();
}

std::thread Core::runMultiThread(std::string game, int num_turns, int num_players) {
    std::thread thread(&Core::play, this, game, num_turns, num_players);
    return thread;
}
