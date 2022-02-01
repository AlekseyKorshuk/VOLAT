#include <iostream>
#include "modules/client/Client.h"
#include "modules/core/Core.h"

using json = nlohmann::json;

int main(int argc, char **argv) {

    string game, name, password;
    int num_turns, num_players;
    if(argc==1){
        name = "VOLAT1";
        password = "";
        game = "testVOLAT";
        num_turns = 15;
        num_players = 1;
    }
    else{
        name = argv[1];
        password = argv[2];
        game = argv[3];
        num_turns = stoi(argv[4]);
        num_players = stoi(argv[5]);
    }

    Core core(name, password);

    /*
    Client client2 = Client();
    client2.login("VOLAT2", "", game);

    Client client3 = Client();
    client3.login("VOLAT3", "", game);
    */

    core.play(game,num_turns,num_players);

    return 0;
}
