#pragma once


#include <iostream>
#include <string>
#include "../map/Map.h"
#include "../content/vehicles/Tank.h"

class Tank;

class Game;

/**
 * Parameters of the sate
 */
union Param {
    /**
     * Constructor
     */
    Param() {
        idp = -1;
    }

    /**
     * Destructor
     */
    ~Param() {}

    /**
     * Constructor tank parameter
     * @param tank Tank
     */
    Param(std::shared_ptr<Tank> tank) : tank(tank) {}

    Position pos;                   // Position
    std::shared_ptr<Tank> tank;     // Tank
    int idp;                        // Parameter ID
};

/**
 * State class
 */
class State {
public:
    /**
     * State constructor
     * @param tank Tank
     * @param game Game
     * @param param Parameters
     */
    State(std::shared_ptr<Tank> tank, std::shared_ptr<Game> game,
          std::shared_ptr<Param> param = std::make_shared<Param>());

    /**
     * Method to get the type
     * @return String type
     */
    virtual std::string getType() = 0;

    /**
     * Method to calculate an action
     * @return String action
     */
    virtual std::string calculateAction() = 0;

    /**
     * Method to perform action from string
     * @param action_s Action
     */
    void doAction(std::string action_s);

    /**
     * Method to convert move to string action
     * @param pos Position
     * @return Resulting string action
     */
    std::string moveToString(Position pos);

    /**
     * Method to convert shoot to string action
     * @param tanks Tanks
     * @return Resulting string action
     */
    std::string shootToString(std::vector<std::shared_ptr<Tank>> tanks);

    /**
     * Method to get priority
     * @return Priority
     */
    int getPriority();

    /**
     * Method to set priority
     * @param priority Priority
     */
    void setPriority(int priority);

    std::shared_ptr<Param> param;   // Parameters

protected:
    std::shared_ptr<Tank> tank;     // Tank
    std::shared_ptr<Game> game;     // Game
    int priority_ = 0;              // Priority
};


