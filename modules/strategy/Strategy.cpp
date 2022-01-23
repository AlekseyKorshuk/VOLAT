#include "Strategy.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Strategy::Strategy() {}

json Strategy::calculate_actions(json state) {
    // TODO calculate actions
    std::string actions = "{\"0\":{\"type\":\"MOVE\",\"data\":{\"vehicle_id\":5,\"target\":{\"x\":-1,\"y\":1,\"z\":0}}},"
                          "\"1\":{\"type\":\"SHOOT\",\"data\":{\"vehicle_id\":5,\"target\":{\"x\":-1,\"y\":1,\"z\":0}}}}";

    return json::parse(actions);
}