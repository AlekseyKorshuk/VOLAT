#pragma once

#include "../map/Map.h"
#include "../content/vehicles/Tank.h"
#include "../player/Player.h"
#include <vector>
#include <map>

class Player;

class Game {
public:
    int idx;                                // ID
    json current_game_state;                // Game state
    int numCalcAction = 0;                  // Number of calculated actions
    std::vector<bool> fCalcAction;          // Whenever action is calculated
    std::vector<std::string> actionList;    // List of actions
    std::vector<Position> param;            // Parameters
    std::vector<std::string> actions;       // Actions

    /**
     * Default constructor
     */
    Game() {};

    /**
     * Constructor
     * @param idx ID
     * @param map_json Map in json format
     * @param state_json Game state in json format
     */
    Game(int idx, json map_json, json state_json);

    /**
     * Update game with json game state
     * @param state_json Game state in json format
     */
    void update(json state_json);

    /**
     * Method to update tank in runtime
     * @param id ID
     * @param x Coordinate X
     * @param y Coordinate Y
     * @param z Coordinate Z
     * @param health Number of health points
     * @param capture_points Number of capture points
     * @param shoot_range_bonus Shooting range bonus from catapult
     */
    void updateTank(int id, int x, int y, int z, int health, int capture_points, int shoot_range_bonus);

    /**
     * Method to update tank in runtime
     * @param id ID
     * @param x Coordinate X
     * @param y Coordinate Y
     * @param z Coordinate Z
     */
    void updateTank(int id, int x, int y, int z);

    /**
     * Method to get tank by ID
     * @param id ID
     * @return Tank
     */
    std::shared_ptr<Tank> getTankByID(int id);

    /**
     * Returns tanks that are under shoot from this tank
     * @param tank Tank
     * @return Tanks
     */
    std::vector<std::vector<std::shared_ptr<Tank> > > tanksUnderShoot(const std::shared_ptr<Tank> &tank);

    /**
     * Returns tanks that can shoot this tank
     * @param player_tank Tank
     * @return Tanks
     */
    std::vector<std::shared_ptr<Tank>> tanksShooting(const std::shared_ptr<Tank> &player_tank);

    /**
     * Method to denote if defence is needed now
     * @param player_tank Tank
     * @return Boolean result
     */
    bool isDefenceNeeded(const std::shared_ptr<Tank> &player_tank);

    /**
     * Method to get capture state
     * @return Capture state in json format
     */
    json getCaptureState();

    /**
     * Method to denote if capture is needed now
     * @param player_tank Tank
     * @return Boolean result
     */
    bool isCaptureNeeded(const std::shared_ptr<Tank> &player_tank);

    /**
     * Method to denote if health is needed now
     * @param player_tank Tank
     * @return Boolean result
     */
    bool isHealthNeeded(const std::shared_ptr<Tank> &player_tank);

    /**
     * Method finds a position from which player_tank can safely shoot opponent_tank
     * @param player_tank Tank
     * @param opponent_tank Tank
     * @param is_zero_danger Check for zero danger
     * @return Resulting positions
     */
    std::vector<Position>
    findSafePositionsToShoot(const std::shared_ptr<Tank> &player_tank, const std::shared_ptr<Tank> &opponent_tank,
                             bool is_zero_danger);

    /**
     * Method finds safe positions with the minimum distance from the specified position
     * @param start Positions
     * @return Resulting positions
     */
    std::vector<Position> findNearestSafePositions(const Position &start);

    /**
     * Method finds tanks that can be killed
     * @param tank Tank
     * @return Resulting tanks
     */
    std::vector<std::shared_ptr<Tank>> GuaranteedKill(const std::shared_ptr<Tank> &tank);

    /**
     * Method finds possible tanks to shoot in specific area
     * @param area Vector of positions
     * @return Resulting tanks
     */
    std::vector<std::shared_ptr<Tank>> findTanksToShootOnArea(const std::vector<Position> &area);

    /**
     * Method finds sorted positions to shoot specific tank
     * @param player_tank Tank
     * @param opponent_tank Tank
     * @return Resulting positions
     */
    std::vector<Position>
    findSortedSafePositionsToShoot(const std::shared_ptr<Tank> &player_tank,
                                   const std::shared_ptr<Tank> &opponent_tank);

    /**
     * Method to get safe shoot action
     * @param player_tank Tank
     * @return Resulting action
     */
    std::string getSafeShootAction(const std::shared_ptr<Tank> &player_tank);

    /**
     * Method that selects best shoot out of a list
     * @param shoots Shoots
     * @param player_tank Tank
     * @param stay_alive Whenever to stay alive
     * @return Resulting tanks
     */
    std::vector<std::shared_ptr<Tank>>
    selectBestShoot(std::vector<std::vector<std::shared_ptr<Tank>>> shoots, const std::shared_ptr<Tank> &player_tank,
                    bool stay_alive);

    /**
     * Method that selects best move out of a list
     * @param moves Moves
     * @param player_tank Tank
     * @return Resulting position to move
     */
    Position selectBestMove(std::vector<Position> moves, std::shared_ptr<Tank> player_tank);

    /**
     * Method to find safe path
     * @param start Starting position
     * @param ends Target positions
     * @return Resulting path
     */
    std::vector<Position>
    findSafePath(const Position &start, std::vector<Position> ends, const std::shared_ptr<Tank> &);

    /**
     * Method to denote if it is possible to kill and stay alive
     * @param player_tank Player tank
     * @return Possible shoot
     */
    std::vector<std::shared_ptr<Tank>> canKillAndStayAlive(const std::shared_ptr<Tank> &player_tank);

    /**
     * Method to denote an action that can save other player tanks
     * @param player_tank Tank
     * @return Resulting action
     */
    std::string saveTeam(const std::shared_ptr<Tank> &player_tank);

    /**
     * Method to find safe positions
     * @param player_tank Tank
     * @param area List of positions
     * @param is_zero_danger Whenever to check for zero danger
     * @param choose_best Whenever to choose the best
     * @return Resulting positions
     */
    std::vector<Position>
    getSafePositions(const std::shared_ptr<Tank> &player_tank, const std::vector<Position> &area, bool is_zero_danger,
                     bool choose_best);

    /**
     * Method to get possible shoots
     * @param player_tank Tank
     * @param check_neutral Whenever to check for neutral rule
     * @return Possible shoots
     */
    std::vector<std::vector<std::shared_ptr<Tank>>>
    getPossibleShoots(const std::shared_ptr<Tank> &player_tank, bool check_neutral);

    /**
     * Method to select best shoot for defence
     * @param shoots Shoots
     * @param player_tank Tank
     * @return Best shoot
     */
    std::vector<std::shared_ptr<Tank>> selectBestShootDefence(std::vector<std::vector<std::shared_ptr<Tank>>> shoots,
                                                              const std::shared_ptr<Tank> &player_tank);

    /**
     * Method to predict behavior of opponents tanks
     */
    void predictingBehaviorOpponentsTanks();

    /**
     * Calculate current state score
     * @return Resulting score
     */
    double calculateCurrentStateScore();

    /**
     * Method to get hexes by specific radius
     * @param radius Radius
     * @return Resulting positions
     */
    std::vector<Position> getHexesByRadius(int radius);

    /**
     * Method to find smart path
     * @param start Starting position
     * @param ends Target positions
     * @param tank Tank
     * @return Resulting path
     */
    std::vector<Position>
    smartFindQuickPath(const Position &start, std::vector<Position> ends, const std::shared_ptr<Tank> &tank);

    /**
     * Method to find quick path with last algorithm
     * @param start Starting position
     * @param ends Target positions
     * @param tank Tank
     * @return Resulting path
     */
    std::vector<Position>
    smartFindQuickPath_l(const Position &start, std::vector<Position> ends, const std::shared_ptr<Tank> &tank);

    /**
     * Method to find stupid path
     * @param start Starting position
     * @param ends Target positions
     * @param tank Tank
     * @return Resulting path
     */
    std::vector<Position>
    stupidFindPath(const Position &start, std::vector<Position> ends, const std::shared_ptr<Tank> &tank);

    /**
     * Method to find stupid path with last algorithm
     * @param start Starting position
     * @param ends Target positions
     * @param tank Tank
     * @return Resulting path
     */
    std::vector<Position>
    stupidFindPath_l(const Position &start, std::vector<Position> ends, const std::shared_ptr<Tank> &tank);

    /**
     * Method to find smart safe path
     * @param start Starting position
     * @param ends Target positions
     * @param tank Tank
     * @return Resulting path
     */
    std::vector<Position>
    smartFindSafePath(const Position &start, std::vector<Position> ends, const std::shared_ptr<Tank> &tank);

    /**
     * Method to find smart safe path with last algorithm
     * @param start Starting position
     * @param ends Target positions
     * @param tank Tank
     * @return Resulting path
     */
    std::vector<Position>
    smartFindSafePath_l(const Position &start, std::vector<Position> ends, const std::shared_ptr<Tank> &tank);

    /**
     * Method to convert shoot to string action
     * @param tank Tank
     * @param tanks Tanks
     * @return Resulting action
     */
    std::string shootToString(std::shared_ptr<Tank> tank, std::vector<std::shared_ptr<Tank>> tanks);

    /**
     * Method to convert move to string action
     * @param tank Tank
     * @param pos Positions
     * @return Resulting action
     */
    std::string moveToString(std::shared_ptr<Tank> tank, Position pos);

    /**
     * Method to perform an action
     * @param tank Tank
     * @param action_s Action string
     */
    void doAction(std::shared_ptr<Tank> tank, std::string action_s);

    /**
     * Method returns player by id
     * @param id ID
     * @return Player
     */
    std::shared_ptr<Player> getPlayer(int id);

    Map map = Map();                                        // Map
    std::vector<std::shared_ptr<Tank>> all_vehicles;        // All vehicles
    std::vector<std::shared_ptr<Tank>> player_vehicles;     // Player vehicles
    std::vector<std::shared_ptr<Tank>> opponent_vehicles;   // Opponent vehicles
    std::vector<std::shared_ptr<Player>> players;           // Players
private:
    int step = 0;                                           // Step number
    Position SPG_pos = {-100, -100, -100};         // SPG position
    Position Heavy_pos = {-100, -100, -100};       // Heavy position
    Position AT_pos = {-100, -100, -100};          // AT-SPG position

    bool state_def = false;                                 // Is state def
    bool SPG_def = false;                                   // Is SPG on defence
    bool Heavy_def = false;                                 // Is Heavy on defence
    bool AT_def = false;                                    // Is AT-SPG on defence

    const int type_find = 1;                                // Type og path finding algorithm

    /**
     * Method to add vehicle
     * @param vehicle Vehicle json
     * @param vehicle_id Vehicle ID
     */
    void addTank(json vehicle, int vehicle_id);

    /**
     * Method to add player
     * @param player_json Player json
     */
    void addPlayer(const json &player_json);

    /**
     * Method to update players
     * @param state_json Game state
     */
    void updatePlayers(json state_json);

    /**
     * Method to predict tank behavior
     * @param tank Tank
     */
    void predictingTankBehavior(const std::shared_ptr<Tank> &tank);

    /**
     * Method to define direction segment
     * @param start Start position
     * @param end End position
     * @return Resulting vector
     */
    std::vector<int> definingDirectionSegments(Position start, Position end);

    /**
     * Method to check for repairs
     * @param tank Tank
     * @param hex Hex
     * @param param Parameter
     */
    void checkingRepairs(const std::shared_ptr<Tank> &tank, std::shared_ptr<Hex> hex, double &param);
};

