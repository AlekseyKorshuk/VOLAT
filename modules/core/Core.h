#pragma once

#include <iostream>
#include <thread>

/**
 * Core class of the game client
 */
class Core {
public:
    /**
     * Constructor
     * @param name Player name
     * @param password Player password
     */
    Core(const std::string &name = "", const std::string &password = "");

    /**
     * Method that starts playing the game
     * @param game Game name
     * @param num_turns Number of turns
     * @param num_players Number of players
     */
    void play(std::string game, int num_turns = 45, int num_players = 3);

    /**
     * Run bot in multithreading mode
     * @param game Game name
     * @param num_turns Number of turns
     * @param num_players Number of players
     * @return Thread
     */
    std::thread runMultiThread(std::string game, int num_turns, int num_players);

private:
    std::string name;       // Player name
    std::string password;   // Player password
};

