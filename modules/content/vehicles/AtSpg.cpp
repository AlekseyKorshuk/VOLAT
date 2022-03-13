#include "AtSpg.h"

const static std::vector<Hex> hex_directions =
        {Hex(1, 0, -1), Hex(1, -1, 0), Hex(0, -1, 1), Hex(-1, 0, 1), Hex(-1, 1, 0), Hex(0, 1, -1)};


AtSpg::AtSpg(int x, int y, int z, int spawn_x, int spawn_y, int spawn_z,
             int health_points, int capture_points, int id)
        : Tank(x, y, z, spawn_x, spawn_y, spawn_z, health_points, capture_points, id) {
    this->type_ = TankType::AT_SPG;
    this->speed_points_ = 1;
    this->damage_ = 1;
    this->max_shot_radius_ = 3;
    this->min_shot_radius_ = 1;
    this->destruction_points_ = 2;
    this->max_health_points_ = 2;
}

std::vector<PosList> AtSpg::getShootingHexesAreas(Map &map) const {
    auto position = map.getHex(getPosition());
    std::vector<PosList> shot_areas;

    std::vector<Position> position_directions =
            {Position(1, 0, -1), Position(1, -1, 0), Position(0, -1, 1), Position(-1, 0, 1), Position(-1, 1, 0),
             Position(0, 1, -1)};


    for (Position pos: position_directions) {
        PosList direction_hexes;
        Position potential_shot_aim = pos_;
        for (int radius = 1; radius <= max_shot_radius_ + shoot_range_bonus_; ++radius) {
            potential_shot_aim += pos;


            std::shared_ptr<Hex> potential_shot_aim_ = map.getHex(potential_shot_aim);

            if (potential_shot_aim_ != nullptr) {
                if (potential_shot_aim_->content->content_type == ContentType::OBSTACLE) {
                    break;
                }
                direction_hexes.push_back(potential_shot_aim);
            }
        }

        if (!direction_hexes.empty()) {
            shot_areas.push_back(direction_hexes);
        }
    }

    return shot_areas;
}
