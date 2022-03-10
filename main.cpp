#include <iostream>
#include "modules/client/Client.h"
#include "modules/core/Core.h"
#include<thread>

using json = nlohmann::json;

int main(int argc, char **argv) {
    std::string game, name, password;
    int num_turns, num_players, is_solo;
    std::thread t1, t2, t3;
    if (argc == 1) {
        name = "VOLAT_MAIN";
        password = "";
        game = "test1234";
        num_turns = 99;
        num_players = 3;
        is_solo = 1;
    } else {
        name = argv[1];
        password = argv[2];
        game = argv[3];
        num_turns = std::stoi(argv[4]);
        num_players = std::stoi(argv[5]);
        is_solo = std::stoi(argv[6]);
    }

    Core core1(name, password);

    if (is_solo == 0) {
        t1 = core1.runMultiThread(game, num_turns, num_players);

        Core core2("VOLAT2", "");
        t2 = core2.runMultiThread(game, num_turns, num_players);

        Core core3("VOLAT3", "");
        t3 = core3.runMultiThread(game, num_turns, num_players);

        t1.join();
        t2.join();
        t3.join();
    }
    else{
        core1.play(game, num_turns, num_players);
    }

    return 0;
}