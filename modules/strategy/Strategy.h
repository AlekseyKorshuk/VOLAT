#pragma once

#include <nlohmann/json.hpp>
#include "../map/Map.h"
using json = nlohmann::json;

class Strategy{
public:
    Strategy(json);

    json calculate_actions(json state);

    Map map;
};
