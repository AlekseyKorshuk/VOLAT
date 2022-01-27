#include "Hex.h"
#include "../content/vehicles/MediumTank.h"

std::vector<Hex> hex_directions = {Hex(1, 0, -1), Hex(1, -1, 0), Hex(0, -1, 1), Hex(-1, 0, 1), Hex(-1, 1, 0),
                                         Hex(0, 1, -1)};

std::vector <Hex> hex_diagonals = {Hex(2, -1, -1), Hex(1, -2, 1), Hex(-1, -1, 2), Hex(-2, 1, 1), Hex(-1, 2, -1),
                                         Hex(1, 1, -2)};

Hex::Hex(int x, int y, int z, ContentType content_type, json data) : x(x), y(y), z(z) {
    this->setHex(content_type, data);
}

Hex &Hex::operator+=(const Hex &hex) {
    this->x += hex.x;
    this->y += hex.y;
    this->z += hex.z;
    return *this;
}

Hex operator+(Hex &a, const Hex &b) {
    return Hex(a.x + b.x, a.y + b.y, a.z + b.z);
}

Hex &Hex::operator-=(const Hex &hex) {
    this->x -= hex.x;
    this->y -= hex.y;
    this->z -= hex.z;
    return *this;
}

Hex operator-(Hex &a, const Hex &b) {
    return Hex(a.x - b.x, a.y - b.y, a.z - b.z);
}


Hex Hex::getDirection(int direction) {
    return hex_directions[direction];
}

Hex Hex::getNeighbor(int direction) {
    return *this + this->getDirection(direction);
}

Hex Hex::getDiagonalNeighbor(int direction) {
    return *this + hex_diagonals[direction];
}

int Hex::getLength() {
    return int((abs(this->x) + abs(this->y) + abs(this->z)) / 2);
}

int Hex::getDistance(Hex hex) {
    return (*this - hex).getLength();
}

void Hex::addNeighbour(Hex* hex){
    this->neighbors.push_back(hex);
}

json Hex::getJson(){
    json j;
    j["x"] = this->x;
    j["y"] = this->y;
    j["z"] = this->z;
    return j;
}

bool Hex::operator==(Hex hex) {
    return this->getJson() == hex.getJson();
}

void Hex::setHex(ContentType content_type, json data) {
    this->is_occupied = true;
    switch (content_type) {
        case ContentType::VEHICLE: {
            std::string temp_vehicle_type = data["vehicle_type"].get<std::string>();
            if (temp_vehicle_type == "medium_tank"){
                MediumTank* tank = new MediumTank(x, y, z, data["health"].get<std::int32_t>(), data["capture_points"].get<std::int32_t>());
                tank->player_id = data["player_id"].get<std::int32_t>();
                this->content = tank;
            }
            break;
        }
        case ContentType::BASE:{
            this->is_occupied = false;
            break;
        }
        default:{
            this->is_occupied = false;
            break;
        }
    }

}
