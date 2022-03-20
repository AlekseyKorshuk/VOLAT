#pragma once

#include <nlohmann/json.hpp>
#include "../map/Map.h"
#include "../game/Game.h"

using json = nlohmann::json;

/**
 * Strategy class
 */
class Strategy {
public:
    /**
     * Strategy constructor
     * @param idx ID
     * @param map_json Map in json format
     * @param state_json Game state in json format
     */
    Strategy(int idx, json map_json, json state_json);

    /**
     * Method to calculate actions
     * @param idx ID
     * @param state Game state
     * @return Resulting json with actions
     */
    json calculate_actions(int idx, json state);

    std::shared_ptr<Game> game; // Game
};
