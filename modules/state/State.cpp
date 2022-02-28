#include "State.h"

State::State(std::shared_ptr<Tank> tank, std::shared_ptr<Game> game, std::shared_ptr<Param> param) : tank(tank),
                                                                                                     game(game),
                                                                                                     param(param) {
};

std::string State::moveToString(Position pos) {
    game->updateTank(tank->id, pos.getX(), pos.getY(), pos.getZ());

    return "{\"type\":\"MOVE\",\"data\":{\"vehicle_id\":" + std::to_string(tank->id) +
           ",\"target\":{\"x\":" + std::to_string(pos.getX()) + ",\"y\":" + std::to_string(pos.getY()) + ",\"z\":" +
           std::to_string(pos.getZ()) + "}}}";
}

std::string State::shootToString(std::vector<std::shared_ptr<Tank>> tanks) {

    Position pos = tanks[0]->getPosition();
    if (tank->getTankType() == TankType::AT_SPG) {
        Position pos_mis_dis = pos;
        int min_dis = 1e9;
        for (auto pos_neighbors: game->map.getHex(tank->getPosition())->neighbors) {

            if (pos_neighbors.getDistance(pos) < min_dis) {
                min_dis = pos_neighbors.getDistance(pos);
                pos_mis_dis = pos_neighbors;
            }
        }
        pos = pos_mis_dis;
    }

    for (auto tank: tanks) {
        if (tank->getHealthPoints() == 1) {
            Position pos = tank->getSpawnPosition();
            game->updateTank(tank->id, pos.getX(), pos.getY(), pos.getZ());
        }
        tank->update(tank->getHealthPoints() - 1);

    }

    return "{\"type\":\"SHOOT\",\"data\":{\"vehicle_id\":" + std::to_string(tank->id) +
           ",\"target\":{\"x\":" + std::to_string(pos.getX()) + ",\"y\":" + std::to_string(pos.getY()) + ",\"z\":" +
           std::to_string(pos.getZ()) + "}}}";
}

