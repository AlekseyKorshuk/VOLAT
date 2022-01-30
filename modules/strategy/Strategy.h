#pragma once

#include <nlohmann/json.hpp>
#include "../map/Map.h"
#include "../game/Game.h"

using json = nlohmann::json;

class Strategy{
public:
    Strategy(json, json);

    json calculate_actions(json state);

    Game game;
};
