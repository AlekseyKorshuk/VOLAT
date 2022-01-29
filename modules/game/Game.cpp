#include "Game.h"
#include "../content/vehicles/MediumTank.h"

Game::Game(json map_json, json state_json)
{
    map = Map(map_json);

    int player_id = state_json["current_player_idx"].get<std::int32_t>();
    for (auto it = state_json["vehicles"].begin(); it != state_json["vehicles"].end(); ++it) {
        int vehicle_id = stoi(it.key());
        json vehicle = it.value();
        json position = vehicle["position"];
        json spawn_position = vehicle["spawn_position"];

        int x = vehicle["position"]["x"].get<std::int32_t>();
        int y = vehicle["position"]["y"].get<std::int32_t>();
        int z = vehicle["position"]["z"].get<std::int32_t>();

        int spawn_x = vehicle["spawn_position"]["x"].get<std::int32_t>();
        int spawn_y = vehicle["spawn_position"]["y"].get<std::int32_t>();
        int spawn_z = vehicle["spawn_position"]["z"].get<std::int32_t>();

        std::string temp_vehicle_type = vehicle["vehicle_type"].get<std::string>();


        std::shared_ptr<Tank> tank;
        if (temp_vehicle_type == "medium_tank") {
           tank = std::make_shared<MediumTank>(
                            x, y, z,
                            spawn_x, spawn_y, spawn_z,
                            vehicle["health"].get<std::int32_t>(),
                            vehicle["capture_points"].get<std::int32_t>(), vehicle_id
                    );
            tank->setPlayerId(vehicle["player_id"].get<std::int32_t>());
        }

        all_vehicles.push_back(tank);

        map.changeOccupied(tank->getPosition(),true);

        if (vehicle["player_id"].get<std::int32_t>() == player_id) {
            player_vehicles.push_back(tank);
        } else {
            opponent_vehicles.push_back(tank);
        }
    }

    json players_json = state_json["players"];
    for (json::iterator it = players_json.begin(); it != players_json.end(); ++it) {
        json player_json = it.value();
        Player player(player_json);

        for (std::shared_ptr<Tank> tank: all_vehicles) {
            if (tank->getPlayerId() == player.id) {
                player.vehicles.push_back(tank);
            }
        }
        players.push_back(player);
    }
}

void Game::update(json state_json) {
    for (auto it = state_json["vehicles"].begin(); it != state_json["vehicles"].end(); ++it) {
        int vehicle_id = stoi(it.key());

        json vehicle = it.value();
        json position = vehicle["position"];

        int x = vehicle["position"]["x"].get<std::int32_t>();
        int y = vehicle["position"]["y"].get<std::int32_t>();
        int z = vehicle["position"]["z"].get<std::int32_t>();

        int capture_points = vehicle["capture_points"].get<std::int32_t>();
        int health = vehicle["health"].get<std::int32_t>();

        updateTank(vehicle_id, x, y, z, health, capture_points);
    }
}


void Game::updateTank(int id, int x, int y, int z) {
    std::shared_ptr<Tank> tank = all_vehicles[id - 1];

    map.changeOccupied(tank->getPosition(),false);
    tank->update(x, y, z);
    map.changeOccupied(tank->getPosition(),true);
}

void Game::updateTank(int id, int x, int y, int z, int health, int capture_points) {
    std::shared_ptr<Tank> tank = all_vehicles[id - 1];

    map.changeOccupied(tank->getPosition(),false);
    tank->update(x, y, z, health, capture_points);
    map.changeOccupied(tank->getPosition(),true);
}