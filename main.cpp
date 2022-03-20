#pragma comment(linker, "/STACK:16777216")

#include "modules/client/Client.h"
#include "modules/core/Core.h"
#include <thread>

using json = nlohmann::json;

int main(int argc, char **argv) {
    std::string game, name, password;
    int num_turns, num_players;
    std::thread t1, t2, t3;
    if (argc == 1) {
        name = "VOLAT";
        password = "";
        game = "test_game_4";
        num_turns = 45;
        num_players = 3;
    } else {
        name = argv[1];
        password = argv[2];
        game = argv[3];
        num_turns = std::stoi(argv[4]);
        num_players = std::stoi(argv[5]);
    }

    Core core1(name, password);
    core1.play(game, num_turns, num_players);

    return 0;
}