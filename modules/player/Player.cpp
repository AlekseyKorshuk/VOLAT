#include "Player.h"


Player::Player(json state) {
    id = state["idx"].get<std::int32_t>();
    name = state["name"].get<std::string>();
}

void Player::update(json state_json) {
    json attack_matrix = state_json["attack_matrix"];
    json win_points = state_json["win_points"];

    for (json::iterator it = win_points.begin(); it != win_points.end(); ++it) {
        json player = it.value();
        int idx = stoi(it.key());
        if (id == idx) {
            point_capture = player["capture"].get<std::int32_t>();
            point_kill = player["kill"].get<std::int32_t>();
        }
    }

    onWhomAttacked.clear();
    whoAttacked.clear();

    for (json::iterator it = attack_matrix.begin(); it != attack_matrix.end(); ++it) {
        json player = it.value();
        int who = stoi(it.key());

        for (json::iterator it = player.begin(); it != player.end(); ++it) {
            int onWhom = it.value().get<std::int32_t>();

            if (who == id) {
                onWhomAttacked.push_back(onWhom);
            }
            if (onWhom == id) {
                whoAttacked.push_back(who);
            }
        }
    }
}
