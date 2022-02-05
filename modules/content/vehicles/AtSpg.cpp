#include "AtSpg.h"

const static std::vector<Hex> hex_directions =
        {Hex(1, 0, -1), Hex(1, -1, 0), Hex(0, -1, 1), Hex(-1, 0, 1), Hex(-1, 1, 0),Hex(0, 1, -1)};


AtSpg::AtSpg(int x, int y, int z, int spawn_x, int spawn_y, int spawn_z,
             int health_points, int capture_points, int id)
    : Tank(x, y, z, spawn_x, spawn_y, spawn_z, health_points, capture_points, id) {
    this->type_ = TankType::AT_SPG;
    this->speed_points_ = 1;
    this->damage_ = 1;
    this->shot_radius_ = 3;
    this->destruction_points_ = 2;
}

std::vector<HexPtrList> AtSpg::getShootingHexesAreas(Map& map) const {
    auto position = map.getHex(getPosition());
    std::vector<HexPtrList> shot_areas;

    for (const Hex hex_direction : hex_directions) {
        HexPtrList direction_hexes;
        Hex potential_shot_aim = *position;
        for (int radius = 1; radius <= 3; ++radius) {
            potential_shot_aim += hex_direction;

            Hex* potential_shot_aim_ =map.getHex(potential_shot_aim);
            if (potential_shot_aim_ != nullptr) {
                direction_hexes.push_back(potential_shot_aim_);
            }
        }

        if (!direction_hexes.empty()) {
            shot_areas.push_back(direction_hexes);
        }
    }

    return shot_areas;
}
