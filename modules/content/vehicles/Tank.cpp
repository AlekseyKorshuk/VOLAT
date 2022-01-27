#include "Tank.h"

Hex Tank::getPosition() const {
    return Hex(x_, y_, z_);
}

Tank::Tank(int x, int y, int z, int health_points, int capture_points) : Content(is_reacheble = false,
                                                                                 content_type = ContentType::VEHICLE),
                                                                         x_(x), y_(y), z_(z),
                                                                         health_points(health_points),
                                                                         capture_points(capture_points) {

}

Tank::Tank(json data) : Content(is_reacheble = false, content_type = ContentType::VEHICLE),
                        x_(data["position"]["x"].get<std::int32_t>()),
                        y_(data["position"]["y"].get<std::int32_t>()),
                        z_(data["position"]["z"].get<std::int32_t>()),
                        health_points(data["health"].get<std::int32_t>()) {
}