#include <iostream>
#include "modules/client/Client.h"
#include "modules/core/Core.h"


using json = nlohmann::json;

int main() {

    string game = "testVOLAT";
    Core core("VOLAT1", "");

    Client client2 = Client();
    client2.login("VOLAT2", "", game);

    Client client3 = Client();
    client3.login("VOLAT3", "", game);


    core.play(game);

    return 0;
}
