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

std::ostream &operator<<(std::ostream &stream, const Tank &tank) {
    stream << tank.getPosition() << " Type: " << static_cast<int>(tank.type_) << ",ID: " << tank.id << ",HP: " << tank.health_points_ << ",CP: " << tank.capture_points_ << ",SP: "
           << tank.speed_points_;
    return stream;
}

TankType Tank::getTankType() {
    return type_;
}