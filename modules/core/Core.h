#pragma once

#include <iostream>


class Core {
public:
    Core(std::string name="", std::string password="");

    void play(std::string game, int num_turns=45, int num_players=3);

private:
    std::string name;
    std::string password;

};

