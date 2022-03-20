#pragma once

#include <vector>
#include "../content/vehicles/Tank.h"
#include <set>

class Tank;

/**
 * Player class
 */
class Player {
public:
    /**
     * Default constructor
     */
    Player() = default;

    /**
     * Constructor from game state json
     * @param state Game state
     */
    Player(json state);

    /**
     * Method that updates data from game state json
     * @param state_json Game state
     */
    void update(json state_json);

    std::string name = "";                          // Player name
    int id = -1;                                    // PLayer ID
    bool is_neutral = false;                        // If player is neutral
    std::vector<std::shared_ptr<Tank>> vehicles;    // Player vehicles
    int point_capture = 0;                          // Number of capture points
    int point_kill = 0;                             // Number of kill points
    std::vector<int> onWhomAttacked;                // Vector of on whom player attacked
    std::vector<int> whoAttacked;                   // Vector of who attacked player
};

