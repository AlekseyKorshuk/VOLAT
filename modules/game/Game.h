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
    int numCalcAction = 0;
    std::vector<bool> fCalcAction;
    std::vector<std::string> actionList;
    std::vector<Position> param;
    std::vector<std::string> actions;

    Game() {};

    Game(int, json, json);

    void update(json);

    void updateTank(int id, int x, int y, int z, int health, int capture_points, int shoot_range_bonus);

    void updateTank(int id, int x, int y, int z);

    std::shared_ptr<Tank> getTankByID(int id);


    json calcSPG_Heavy_At(std::shared_ptr<Tank> spg_tank, std::shared_ptr<Tank> heavy_tank, std::shared_ptr<Tank> at_spg_tank);

    json calcLightMedium(std::shared_ptr<Tank> light_tank, std::shared_ptr<Tank> medium_tank);

    //Return tanks that are under shoot from this tank
    std::vector<std::vector<std::shared_ptr<Tank> > > tanksUnderShoot(const std::shared_ptr<Tank> &);

    std::vector<std::shared_ptr<Tank>> tanksShooting(const std::shared_ptr<Tank> &);

    bool isDefenceNeeded(const std::shared_ptr<Tank> &);

    json getCaptureState();

    bool isCaptureNeeded(const std::shared_ptr<Tank> &player_tank);

    bool isHealthNeeded(const std::shared_ptr<Tank> &player_tank);


    //Finds a position from which tank_1 can safely shoot tank_2
    std::vector<Position>
    findSafePositionsToShoot(const std::shared_ptr<Tank> &, const std::shared_ptr<Tank> &, bool is_zero_danger);

    //Finds safe positions with the minimum distance from the specified position
    std::vector<Position> findNearestSafePositions(const Position &);

    std::vector<std::shared_ptr<Tank>> GuaranteedKill(const std::shared_ptr<Tank> &);

    //Finds possible tanks to shoot in specific area
    std::vector<std::shared_ptr<Tank>> findTanksToShootOnArea(const std::vector<Position> &);

    //Finds sorted positions to shoot specific tank
    std::vector<Position>
    findSortedSafePositionsToShoot(const std::shared_ptr<Tank> &player_tank,
                                   const std::shared_ptr<Tank> &opponent_tank);

    std::string getSafeShootAction(const std::shared_ptr<Tank> &player_tank);

    std::vector<std::shared_ptr<Tank>>
    selectBestShoot(std::vector<std::vector<std::shared_ptr<Tank>>> shoots, const std::shared_ptr<Tank> &player_tank,
                    bool stay_alive);

    Position selectBestMove(std::vector<Position> moves, std::shared_ptr<Tank> player_tank);


    std::vector<Position>
    findSafePath(const Position &start, std::vector<Position> ends, const std::shared_ptr<Tank> &);

    std::vector<std::shared_ptr<Tank>> canKillAndStayAlive(const std::shared_ptr<Tank> &player_tank);

    std::string saveTeam(const std::shared_ptr<Tank> &player_tank);


    std::vector<Position>
    getSafePositions(const std::shared_ptr<Tank> &player_tank, const std::vector<Position> &area, bool is_zero_danger,
                     bool choose_best);

    std::vector<std::vector<std::shared_ptr<Tank>>> getPossibleShoots(const std::shared_ptr<Tank> &player_tank, bool check_neutral);

    std::vector<std::shared_ptr<Tank>> selectBestShootDefence(std::vector<std::vector<std::shared_ptr<Tank>>> shoots,
                                                              const std::shared_ptr<Tank> &player_tank);

    void predictingBehaviorOpponentsTanks();

    double calculateCurrentStateScore();

    std::vector<Position> getHexesByRadius(int radius);

    std::vector<Position>
    smartFindQuickPath(const Position &start, std::vector<Position> ends, const std::shared_ptr<Tank> &tank);

    std::vector<Position>
    smartFindQuickPath_l(const Position &start, std::vector<Position> ends, const std::shared_ptr<Tank> &tank);

    std::vector<Position>
    stupidFindPath(const Position &start, std::vector<Position> ends, const std::shared_ptr<Tank> &tank);

    std::vector<Position>
    stupidFindPath_l(const Position &start, std::vector<Position> ends, const std::shared_ptr<Tank> &tank);

    std::vector<Position>
    smartFindSafePath(const Position &start, std::vector<Position> ends, const std::shared_ptr<Tank> &tank);

    std::vector<Position>
    smartFindSafePath_l(const Position &start, std::vector<Position> ends, const std::shared_ptr<Tank> &tank);


    std::string shootToString(std::shared_ptr<Tank> tank, std::vector<std::shared_ptr<Tank>> tanks);

    std::string moveToString(std::shared_ptr<Tank> tank, Position pos);

    void doAction(std::shared_ptr<Tank> tank, std::string action_s);



    Map map = Map();

    std::vector<std::shared_ptr<Tank>> all_vehicles;
    std::vector<std::shared_ptr<Tank>> player_vehicles;
    std::vector<std::shared_ptr<Tank>> opponent_vehicles;
    std::vector<std::shared_ptr<Player>> players;

    std::shared_ptr<Player> getPlayer(int);

private:
    int step = 0;
    Position SPG_pos = {-100, -100, -100};
    Position Heavy_pos = {-100, -100, -100};
    Position AT_pos = {-100, -100, -100};

    bool state_def = false;
    bool SPG_def = false;
    bool Heavy_def = false;
    bool AT_def = false;

    const int type_find = 1;

    void addTank(json, int);

    void addPlayer(const json &);

    void updatePlayers(json);


    void predictingTankBehavior(const std::shared_ptr<Tank> &);

    std::vector<int> definingDirectionSegments(Position, Position);

    void checkingRepairs(const std::shared_ptr<Tank> &tank, std::shared_ptr<Hex> hex, double& param);
};

