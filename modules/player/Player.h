#pragma once

#include <vector>
#include "../content/vehicles/Tank.h"
#include <set>
class Tank;

class Player {
public:
    Player() = default;

    Player(json state);

    void update(json);

    std::string name = "";
    int id = -1;

    bool is_neutral = false;

    std::vector<std::shared_ptr<Tank>> vehicles;

    int point_capture = 0;
    int point_kill = 0;

    std::vector<int> onWhomAttacked;
    std::vector<int> whoAttacked;
};

