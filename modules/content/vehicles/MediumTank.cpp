#include "MediumTank.h"
#include <set>

const static std::vector<Hex> hex_directions =
        {Hex(1, 0, -1), Hex(1, -1, 0), Hex(0, -1, 1), Hex(-1, 0, 1), Hex(-1, 1, 0), Hex(0, 1, -1)};

HexList MediumTank::getAvailableHexesForMove(const Map& map) const {
    std::set<Hex> hex_moves;
    std::vector<Hex> neighbors;
    auto position = map.getHex(getPosition());

    for (size_t direction = 0; direction < hex_directions.size(); ++direction) {
        auto neighbor = position->getNeighbor(direction);

        if (map.belongs(neighbor)) {
            neighbors.push_back(neighbor);
            hex_moves.insert(neighbor);
        }
    }

    for (auto& neighbor : neighbors) {
        for (size_t direction = 0; direction < hex_directions.size(); ++direction) {
            auto next_neighbor = neighbor.getNeighbor(direction);
            hex_moves.insert(next_neighbor);
        }
    }
    hex_moves.erase(*position);

    return {hex_moves.begin(), hex_moves.end()};
}

std::vector<HexList> MediumTank::getShootingHexesAreas(const Map& map) const {
    const HexList hex_moves = getAvailableHexesForMove(map);
    std::vector<HexList> areas;
    Hex position = getPosition();

    for (const auto& hex : hex_moves) {
        if (position.getDistance(hex) == 2) {
            areas.push_back(HexList(1, hex));
        }
    }

    return areas;
}

MediumTank::MediumTank(int x, int y, int z, int health_points, int capture_points, int id)
    : Tank(x, y, z, health_points, capture_points, id) {
    this->type_ = TankType::MEDIUM;
    this->speed_points_ = 2;
    this->damage_ = 1;
    this->destruction_points_ = 2;
}