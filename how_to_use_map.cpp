#include <iostream>
#include "modules/client/Client.h"
#include "modules/core/Core.h"

#include "modules/content/vehicles/MediumTank.h"
#include "modules/map/Map.h"

using json = nlohmann::json;

int main() {

    string game = "testVOLAT";
    Client client1 = Client();
    client1.login("VOLAT1", "", game);

    Client client2 = Client();
    client2.login("VOLAT2", "", game);

    Client client3 = Client();
    client3.login("VOLAT3", "", game);

    Map map(client1.map().msg);

    map.setMap(client1.game_state().msg);

    for (auto it: map.findPath(Hex(-3, 10, -7), Hex(-7, 9, -2))) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    for (auto it: map.player_vehicles) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    for (auto it: map.opponent_vehicles) {
        std::shared_ptr<Tank> c = static_pointer_cast<Tank>(it->content);
        std::cout << *c << "\n";
    }
    std::cout << std::endl;
    for (auto it: map.opponent_vehicles) {
        std::shared_ptr<Tank> c = static_pointer_cast<Tank>(it->content);
        std::cout << *c << "\n";
    }

    map.setMap(client1.game_state().msg);
    std::cout << std::endl;
    for (auto it: map.opponent_vehicles) {
        std::shared_ptr<Tank> c = static_pointer_cast<Tank>(it->content);
        std::cout << *c << "\n";
    }


    return 0;
}
