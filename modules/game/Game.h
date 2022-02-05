#pragma once
#include "../map/Map.h"
#include "../content/vehicles/Tank.h"
#include "../player/Player.h"
#include <vector>
#include <map>

class Player;

class Game {
public:
    int idx;
    Game() {};
    Game(int, json , json);
    void update(json);

    void updateTank(int id, int x, int y, int z, int health, int capture_points);
    void updateTank(int id, int x, int y, int z);



    //returns tanks that are under shoot from this tank
    std::vector<std::vector<std::shared_ptr<Tank> > > tanksUnderShoot(std::shared_ptr<Tank>);



    Map map = Map();

    std::vector<std::shared_ptr<Tank>> all_vehicles;
    std::vector<std::shared_ptr<Tank>> player_vehicles;
    std::vector<std::shared_ptr<Tank>> opponent_vehicles;

    std::map<int, Player> players;
private:
    void addTank(json, int);
    void addPlayer(json);
};

