#include "Tank.h"

void Tank::setPlayerId(int player_id) {
    player_id_ = player_id;
}

int Tank::getPlayerId() {
    return player_id_;
}

Hex Tank::getPosition() const {
    return Hex(x_, y_, z_);
}

void Tank::update(int x, int y, int z, int health, int capture_points) {
    x_=x;
    y_=y;
    z_=z;
    health_points_ = health;
    capture_points_ = capture_points;
}

void Tank::update(int x, int y, int z) {
    x_=x;
    y_=y;
    z_=z;
}

void Tank::update(int health) {
    health_points_ = health;
}

void Tank::update(Hex hex) {
    this->update(hex.x, hex.y, hex.z);
}

void Tank::update(Hex* hex) {
    this->update(*hex);
}

Tank::Tank(int x, int y, int z, int spawn_x, int spawn_y, int spawn_z, int health_points, int capture_points, int id)
    : Content(is_reacheble = false
    , content_type = ContentType::VEHICLE
    , id=id)
    , x_(x), y_(y), z_(z)
    , spawn_x_(spawn_x), spawn_y_(spawn_y), spawn_z_(spawn_z)
    , health_points_(health_points)
    , capture_points_(capture_points)
{
}

Tank::Tank(json data, int id)
    : Content(is_reacheble = false, content_type = ContentType::VEHICLE, id=id)
    , x_(data["position"]["x"].get<std::int32_t>())
    , y_(data["position"]["y"].get<std::int32_t>())
    , z_(data["position"]["z"].get<std::int32_t>())
    , health_points_(data["health"].get<std::int32_t>())
{
}

HexPtrList Tank::getAchievableHexes(Map& map) const {
    auto position = map.getHex(getPosition());
    std::vector<Hex *> hexes;
    std::vector<Hex *> visited;

    std::queue<std::pair<Hex *, int> > Queue;
    Queue.push({position, 0});

    while(!Queue.empty()) {
        Hex *current_node = Queue.front().first;
        int current_dist = Queue.front().second;

        Queue.pop();
        if (current_dist != speed_points_) {
            for (Hex *node: current_node->neighbors) {
                if (std::find_if(visited.begin(), visited.end(),
                                 [&node](Hex *hex) { return hex->getJson() == node->getJson(); }) ==
                        visited.end()) {
                    if (node->content != nullptr && node->content->is_reacheble) {
                        Queue.push({node, current_dist + 1});
                        hexes.push_back(node);
                    }
                }
            }
        }
    }

    return hexes;
}

HexPtrList Tank::getHexesInShotRadius(Map& map) const {
    auto position = map.getHex(getPosition());
    std::vector<Hex *> hexes;

    std::queue<std::pair<Hex *, int> > Queue;
    Queue.push({position, 0});

    while(!Queue.empty()) {
        Hex *current_node = Queue.front().first;
        int current_dist = Queue.front().second;

        Queue.pop();
        if (current_dist != speed_points_) {
            for (Hex *node: current_node->neighbors) {
                if (!node->visited && node->content != nullptr && node->content->is_reacheble) {
                    node->visited = true;
                    Queue.push({node, current_dist + 1});
                    hexes.push_back(node);
                }
            }
        }
    }

    return hexes;
}

std::vector<HexPtrList> Tank::getShootingHexesAreas(Map& map) const {
    auto hexes_in_shot_radius = getHexesInShotRadius(map);
    std::vector<HexPtrList> shot_areas;
    Hex position = getPosition();

    for (const auto& hex : hexes_in_shot_radius) {
        if (position.getDistance(*hex) == shot_radius_) {
            shot_areas.push_back(HexPtrList(1, hex));
        }
    }
    map.clearPath();

    return shot_areas;
}

std::ostream &operator<<(std::ostream &stream, const Tank &tank) {
    stream << tank.getPosition() << " Type: " << int(tank.type_) << ",ID: " << tank.id << ",HP: " << tank.health_points_ << ",CP: " << tank.capture_points_ << ",SP: "
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