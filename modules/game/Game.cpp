#include "Game.h"
#include "../content/vehicles/MediumTank.h"

Game::Game(int idx, json map_json, json state_json) : idx(idx)
{
    map = Map(map_json);

    for (int i = 0; i < 15; i++) all_vehicles.push_back(nullptr);

    json players_json = state_json["players"];
    for (json::iterator it = players_json.begin(); it != players_json.end(); ++it) {
        json player_json = it.value();
        addPlayer(player_json);
    }



    for (auto it = state_json["vehicles"].begin(); it != state_json["vehicles"].end(); ++it) {
        int vehicle_id = stoi(it.key());
        json vehicle = it.value();
        addTank(vehicle, vehicle_id);
    }

}

void Game::update(json state_json) {

    json players_json = state_json["players"];
    int k = 0;
    for (json::iterator it = players_json.begin(); it != players_json.end(); ++it) {
        k++;

        if (k > players.size()) {
            json player_json = it.value();
            addPlayer(player_json);
        } else {
            //updatePlayer(player_json);
        }
    }


    for (auto it = state_json["vehicles"].begin(); it != state_json["vehicles"].end(); ++it) {
        int vehicle_id = stoi(it.key());
        json vehicle = it.value();

        if (all_vehicles[vehicle_id - 1] == nullptr) {
            addTank(vehicle, vehicle_id);
        } else {


            json position = vehicle["position"];

            int x = vehicle["position"]["x"].get<std::int32_t>();
            int y = vehicle["position"]["y"].get<std::int32_t>();
            int z = vehicle["position"]["z"].get<std::int32_t>();

            int capture_points = vehicle["capture_points"].get<std::int32_t>();
            int health = vehicle["health"].get<std::int32_t>();

            updateTank(vehicle_id, x, y, z, health, capture_points);
        }
    }
}


void Game::addTank(json vehicle, int vehicle_id) {

    json position = vehicle["position"];
    json spawn_position = vehicle["spawn_position"];

    int x = vehicle["position"]["x"].get<std::int32_t>();
    int y = vehicle["position"]["y"].get<std::int32_t>();
    int z = vehicle["position"]["z"].get<std::int32_t>();

    int spawn_x = vehicle["spawn_position"]["x"].get<std::int32_t>();
    int spawn_y = vehicle["spawn_position"]["y"].get<std::int32_t>();
    int spawn_z = vehicle["spawn_position"]["z"].get<std::int32_t>();

    std::string temp_vehicle_type = vehicle["vehicle_type"].get<std::string>();


    std::shared_ptr<Tank> tank = nullptr;
    if (temp_vehicle_type == "medium_tank") {
        tank = std::make_shared<MediumTank>(
                x, y, z,
                spawn_x, spawn_y, spawn_z,
                vehicle["health"].get<std::int32_t>(),
                vehicle["capture_points"].get<std::int32_t>(), vehicle_id
        );
        tank->setPlayerId(vehicle["player_id"].get<std::int32_t>());
    }

    all_vehicles[vehicle_id - 1] = tank;


    if (tank != nullptr) {
        map.changeOccupied(tank->getPosition(),true);

        if (vehicle["player_id"].get<std::int32_t>() == idx) {
            player_vehicles.push_back(tank);
        } else {
            opponent_vehicles.push_back(tank);
        }

        for (int i = 0; i < players.size(); i++) {
            if (tank->getPlayerId() == players[i].id) {
                players[i].vehicles.push_back(tank);
            }
        }
    }

}

void Game::addPlayer(json player_json) {
    Player player(player_json);
    players.push_back(player);
}


void Game::updateTank(int id, int x, int y, int z) {
    std::shared_ptr<Tank> tank = all_vehicles[id - 1];

    if (tank != nullptr) {
        map.changeOccupied(tank->getPosition(),false);
        tank->update(x, y, z);
        map.changeOccupied(tank->getPosition(),true);
    }
}

void Game::updateTank(int id, int x, int y, int z, int health, int capture_points) {
    std::shared_ptr<Tank> tank = all_vehicles[id - 1];

    if (tank != nullptr) {


        map.changeOccupied(tank->getPosition(),false);
        tank->update(x, y, z, health, capture_points);
        map.changeOccupied(tank->getPosition(),true);
    }
}


std::vector<std::vector<std::shared_ptr<Tank> > > Game::tanksUnderShoot(std::shared_ptr <Tank> tank) {
    std::vector<std::vector<std::shared_ptr<Tank> > > tanks;

    std::vector<std::vector<Hex> >  shootingHexesAreas = tank->getShootingHexesAreas(map);

    for (auto hexList: shootingHexesAreas) {
        for (auto hex: hexList) {
            std::vector<std::shared_ptr<Tank> > tankList;
            for (auto tank: opponent_vehicles){
                if (hex == tank->getPosition() && tank->getHealthPoints() != 0) {
                    tankList.push_back(tank);
                }
            }
            if (!tankList.empty()) {
                tanks.push_back(tankList);
            }
        }
    }

    return tanks;
}