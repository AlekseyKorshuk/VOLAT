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

    json current_game_state;

    Game() {};

    Game(int, json, json);

    void update(json);

    void updateTank(int id, int x, int y, int z, int health, int capture_points, int shoot_range_bonus);

    void updateTank(int id, int x, int y, int z);

    std::shared_ptr<Tank> getTankByID(int id);

    //Return tanks that are under shoot from this tank
    std::vector<std::vector<std::shared_ptr<Tank> > > tanksUnderShoot(std::shared_ptr<Tank>);

    bool isDefenceNeeded(std::shared_ptr<Tank>);

    //Finds a position from which tank_1 can safely shoot tank_2
    std::vector<Position> findSafePositionsToShoot(std::shared_ptr<Tank>, std::shared_ptr<Tank>);

    //Finds safe positions with the minimum distance from the specified position
    std::vector<Position> findNearestSafePositions(Position);

    std::vector<std::shared_ptr<Tank>> GuaranteedKill(std::shared_ptr<Tank>);

    //Finds possible tanks to shoot in specific area
    std::vector<std::shared_ptr<Tank>> findTanksToShootOnArea(std::vector<Position>);

    //Finds sorted positions to shoot specific tank
    std::vector<Position>
    findSortedSafePositionsToShoot(std::shared_ptr<Tank> player_tank, std::shared_ptr<Tank> opponent_tank);


    std::vector<Position>
    findSafePath(Position start, std::vector<Position> ends, std::shared_ptr<Tank>);

    std::vector<std::shared_ptr<Tank>> canKillAndStayAlive(const std::shared_ptr<Tank>& player_tank);

    Map map = Map();

    std::vector<std::shared_ptr<Tank>> all_vehicles;
    std::vector<std::shared_ptr<Tank>> player_vehicles;
    std::vector<std::shared_ptr<Tank>> opponent_vehicles;
    std::vector<std::shared_ptr<Player>> players;

    std::shared_ptr<Player> getPlayer(int);

private:
    void addTank(json, int);

    void addPlayer(json);

    void updatePlayers(json);


    void predictingBehaviorOpponentsTanks();

    void predictingTankBehavior(std::shared_ptr<Tank>);

    std::vector<int> definingDirectionSegments(Position, Position);
};

