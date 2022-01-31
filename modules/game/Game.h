#pragma once
#include "..\map\Map.h"
#include "../content/vehicles/Tank.h"
#include "../player/Player.h"
#include <vector>

class Player;

class Game {
public:
    int idx;
    Game() {};
    Game(int, json , json);
    void update(json);

    void updateTank(int id, int x, int y, int z, int health, int capture_points);
    void updateTank(int id, int x, int y, int z);

    Map map = Map();

    std::vector<std::shared_ptr<Tank>> all_vehicles;
    std::vector<std::shared_ptr<Tank>> player_vehicles;
    std::vector<std::shared_ptr<Tank>> opponent_vehicles;

    std::vector<Player> players;
private:
    void addTank(json, int);
    void addPlayer(json);

};

