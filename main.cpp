#include <iostream>
#include "modules/client/Client.h"
#include "modules/core/Core.h"
#include<thread>

using json = nlohmann::json;

int main(int argc, char **argv) {
    std::string game, name, password;
    int num_turns, num_players;
    if (argc == 1) {
        name = "VOLAT1";
        password = "";
        game = "testVOLATAlex";
        num_turns = 99;
        num_players = 3;
    } else {
        name = argv[1];
        password = argv[2];
        game = argv[3];
        num_turns = std::stoi(argv[4]);
        num_players = std::stoi(argv[5]);
    }

    Core core1(name, password);
    std::thread t1 = core1.runMultiThread(game, num_turns, num_players);

    Core core2("VOLAT2", "");
    std::thread t2 = core2.runMultiThread(game, num_turns, num_players);

    Core core3("VOLAT3", "");
    std::thread t3 = core3.runMultiThread(game, num_turns, num_players);

    t1.join();
    t2.join();
    t3.join();

    return 0;
}