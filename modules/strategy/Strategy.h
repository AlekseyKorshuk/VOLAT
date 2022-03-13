#pragma once

#include <nlohmann/json.hpp>
#include "../map/Map.h"
#include "../game/Game.h"

using json = nlohmann::json;

class Strategy {
public:
    Strategy(int idx, json, json);

    json calculate_actions_old(int idx, json state);


    json calculate_actions(int idx, json state);

    std::shared_ptr<Game> game;
};
