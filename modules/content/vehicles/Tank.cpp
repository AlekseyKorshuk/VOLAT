#include "Tank.h"

#include <utility>

void Tank::setPlayerId(int player_id) {
    player_id_ = player_id;
}

int Tank::getPlayerId() {
    return player_id_;
}

Position Tank::getPosition() const {
    return pos_;
}

void Tank::update(int x, int y, int z, int health, int capture_points, int shoot_range_bonus) {
    update(x, y, z);

    if (pos_ == spawn_pos_) {
        list_moves_.clear();
    } else {
        list_moves_.push_back(Position(x, y, z));
    }
    health_points_ = health;
    capture_points_ = capture_points;
    shoot_range_bonus_ = shoot_range_bonus;
}

void Tank::update(int x, int y, int z) {
    pos_ = Position(x, y, z);
}

void Tank::update(int health) {
    health_points_ = health;
}

void Tank::update(Position pos) {
    this->update(pos.getX(), pos.getY(), pos.getZ());
}

Tank::Tank(int x, int y, int z, int spawn_x, int spawn_y, int spawn_z, int health_points, int capture_points, int id)
        : Content(is_reacheble = false, content_type = ContentType::VEHICLE, id = id),
          pos_(x, y, z),
          spawn_pos_(spawn_x, spawn_y, spawn_z), health_points_(health_points),
          capture_points_(capture_points), current_strategy_(nullptr) {
}

Tank::Tank(json data, int id)
        : Content(is_reacheble = false, content_type = ContentType::VEHICLE, id = id),
          pos_(data["position"]["x"].get<std::int32_t>(),
               data["position"]["y"].get<std::int32_t>(),
               data["position"]["z"].get<std::int32_t>()),
          spawn_pos_(data["spawn_position"]["x"].get<std::int32_t>(),
                     data["spawn_position"]["y"].get<std::int32_t>(),
                     data["spawn_position"]["z"].get<std::int32_t>()),
          health_points_(data["health"].get<std::int32_t>()),
          current_strategy_(nullptr) {
}

PosList Tank::getAchievableHexes(Map &map) const {
    std::vector<Position> hexes;

    map.num_visited++;

    std::queue<std::pair<Position, int> > Queue;
    Queue.push({pos_, 0});

    while (!Queue.empty()) {
        Position current_pos = Queue.front().first;
        int current_dist = Queue.front().second;

        Queue.pop();
        if (current_dist != speed_points_) {
            for (Position pos: map.getHex(current_pos)->neighbors) {
                std::shared_ptr<Hex> node = map.getHex(pos);
                if (node->visited_d < map.num_visited) {
                    node->visited_d = map.num_visited;
                    if (node->content != nullptr && node->content->content_type != ContentType::OBSTACLE) {
                        Queue.push({pos, current_dist + 1});
                        hexes.push_back(pos);
                    }
                }
            }
        }
    }

    return hexes;
}

std::vector<PosList> Tank::getShootingHexesAreas(Map &map) const {
    std::vector<PosList> hexes;

    map.num_visited++;
    map.getHex(pos_)->visited_d = map.num_visited;
    std::queue<std::pair<Position, int> > Queue;
    Queue.push({pos_, 0});

    while (!Queue.empty()) {
        Position current_pos = Queue.front().first;
        int current_dist = Queue.front().second;

        Queue.pop();
        if (current_dist != max_shot_radius_ + shoot_range_bonus_) {
            for (Position pos: map.getHex(current_pos)->neighbors) {
                std::shared_ptr<Hex> node = map.getHex(pos);
                if (node->visited_d < map.num_visited) {
                    node->visited_d = map.num_visited;
                    if (node->content != nullptr) {
                        Queue.push({pos, current_dist + 1});

                        if (current_dist + 1 >= min_shot_radius_ &&
                            current_dist + 1 <= max_shot_radius_ + shoot_range_bonus_) {
                            hexes.push_back({pos});
                        }
                    }
                }
            }
        }
    }

    return hexes;
}

std::ostream &operator<<(std::ostream &stream, const Tank &tank) {
    stream << tank.getPosition() << " Type: " << int(tank.type_) << ",ID: " << tank.id << ",HP: " << tank.health_points_
           << ",CP: " << tank.capture_points_ << ",SP: "
           << tank.speed_points_;
    return stream;
}

TankType Tank::getTankType() {
    return type_;
}

int Tank::getSpeedPoints() {
    return speed_points_;
}

int Tank::getHealthPoints() {
    return health_points_;
}

int Tank::getDamage() {
    return damage_;
}

int Tank::getDestructionPoints() {
    return destruction_points_;
}

int Tank::getCapturePoints() {
    return capture_points_;
}

Position Tank::getSpawnPosition() {
    return spawn_pos_;
}

std::string Tank::getStringTankType() {
    switch (getTankType()) {
        case TankType::SPG:
            return "SPG";
        case TankType::AT_SPG:
            return "AT_SPG";
        case TankType::MEDIUM:
            return "MEDIUM";
        case TankType::LIGHT:
            return "LIGHT";
        case TankType::HEAVY:
            return "HEAVY";
        default:
            return "[Unknown TankType]";
    }
}

int Tank::getMaxHealthPoints() {
    return this->max_health_points_;
}
