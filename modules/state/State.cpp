#include "State.h"

State::State(std::shared_ptr<Tank> tank, std::shared_ptr<Game> game, std::shared_ptr<Param> param) : tank(tank),
                                                                                                     game(game),
                                                                                                     param(param) {
};

std::string State::moveToString(Position pos) {
    if (pos == tank->getPosition())
        return "";

    Position tank_pos = tank->getPosition();
    game->updateTank(tank->id, pos.getX(), pos.getY(), pos.getZ());
    param->pos = tank_pos;
    game->map.changeOccupied(tank_pos, true);

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
            Position sp_pos = tank->getSpawnPosition();
            Position pos = tank->getPosition();
            game->updateTank(tank->id, sp_pos.getX(), sp_pos.getY(), sp_pos.getZ());
            game->map.changeOccupied(pos, true);
        }
        tank->update(tank->getHealthPoints() - 1);

    }

    return "{\"type\":\"SHOOT\",\"data\":{\"vehicle_id\":" + std::to_string(tank->id) +
           ",\"target\":{\"x\":" + std::to_string(pos.getX()) + ",\"y\":" + std::to_string(pos.getY()) + ",\"z\":" +
           std::to_string(pos.getZ()) + "}}}";
}

void State::doAction(std::string action_s) {
    if (action_s.empty()) return;

    json action = json::parse(action_s);
    Position pos = Position(
            action["data"]["target"]["x"].get<std::int32_t>(),
            action["data"]["target"]["y"].get<std::int32_t>(),
            action["data"]["target"]["z"].get<std::int32_t>()
    );
    if (action["type"].get<std::string>() == "MOVE") {
        game->map.changeOccupied(param->pos, false);
    } else {
        bool f = true;
        for (auto tank: game->opponent_vehicles) {
            if (tank->getPosition() == pos) {
                f = false;
                break;
            }
        }
        if (f) {
            game->map.changeOccupied(pos, false);
        }
    }
}

int State::getPriority() {
    return priority_;
}

void State::setPriority(int priority) {
    priority_ = priority;
}