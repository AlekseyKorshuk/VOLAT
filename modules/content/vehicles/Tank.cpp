#include "Tank.h"

void Tank::setPlayerId(int player_id) {
    player_id_ = player_id;
}

Hex Tank::getPosition() const {
    return Hex(x_, y_, z_);
}

Tank::Tank(int x, int y, int z, int health_points, int capture_points)
    : Content(is_reacheble = false
    , content_type = ContentType::VEHICLE)
    , x_(x), y_(y), z_(z)
    , health_points_(health_points)
    , capture_points_(capture_points)
{
}

Tank::Tank(json data)
    : Content(is_reacheble = false, content_type = ContentType::VEHICLE)
    , x_(data["position"]["x"].get<std::int32_t>())
    , y_(data["position"]["y"].get<std::int32_t>())
    , z_(data["position"]["z"].get<std::int32_t>())
    , health_points_(data["health"].get<std::int32_t>())
{
}

std::ostream &operator<<(std::ostream &stream, const Tank &tank) {
    stream << tank.getPosition() << " HP: " << tank.health_points_ << ", CP: " << tank.capture_points_ << ", SP: "
           << tank.speed_points_;
    return stream;
}