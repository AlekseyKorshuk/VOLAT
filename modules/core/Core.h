#pragma once

#include <iostream>
#include <thread>


class Core {
public:
    Core(const std::string &name = "", const std::string &password = "");

    void play(std::string game, int num_turns = 45, int num_players = 3);

    std::thread runMultiThread(std::string game, int num_turns, int num_players);

private:
    std::string name;
    std::string password;
};

