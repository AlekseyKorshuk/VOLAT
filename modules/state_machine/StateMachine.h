#pragma once

#include "../map/Map.h"
#include "../content/vehicles/Tank.h"
#include "../state/State.h"
#include "../game/Game.h"

class Map;

class Tank;

class State;

/**
 * State machine class
 */
class StateMachine {
public:
    /**
     * Constructor
     * @param tank Tank
     * @param game Game
     */
    StateMachine(std::shared_ptr<Tank> tank, std::shared_ptr<Game> game);

    /**
     * Method that updates tank state
     */
    virtual void updateState() = 0;

    /**
     * Method that calculates action
     * @return Resulting action
     */
    std::string calculateAction();

    /**
     * Method that changes state
     * @param new_state New game state
     */
    void changeState(std::shared_ptr<State> new_state);

    /**
     * Returns state name
     * @return String game state
     */
    std::string getStateName();

    /**
     * Returns state
     * @return State
     */
    std::shared_ptr<State> getState();

    /**
     * Returns priority of the tank in the decision queue
     * @return Priority
     */
    int getPriority();

    /**
     * Method that performs action from string
     * @param s String action
     */
    void doAction(std::string s);

protected:
    std::shared_ptr<State> state = nullptr; // Tank state
    std::shared_ptr<Game> game = nullptr;   // Game
    std::shared_ptr<Tank> tank = nullptr;   // Tank
};

