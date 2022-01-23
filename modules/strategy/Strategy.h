#pragma once

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Strategy{
public:
    Strategy();

    json calculate_actions(json state);


};
