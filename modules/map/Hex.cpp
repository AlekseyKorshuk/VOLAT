#include "Hex.h"

const std::vector<Hex> hex_directions = {Hex(1, 0, -1), Hex(1, -1, 0), Hex(0, -1, 1), Hex(-1, 0, 1), Hex(-1, 1, 0),
                                         Hex(0, 1, -1)};

const std::vector <Hex> hex_diagonals = {Hex(2, -1, -1), Hex(1, -2, 1), Hex(-1, -1, 2), Hex(-2, 1, 1), Hex(-1, 2, -1),
                                    Hex(1, 1, -2)};


Hex::Hex(int x, int y, int z, bool is_occupied, json data) : x(x), y(y), z(z) {
    if (x + y + x != 0) throw "x + y + z must be 0";
}

Hex &Hex::operator+=(const Hex &hex) {
    this->x += hex.x;
    this->y += hex.y;
    this->z += hex.z;
    return *this;
}

Hex operator+(Hex &a, const Hex &b) {
    return Hex(a.x + b.x, a.y + b.y, a.y + b.y);
}

Hex &Hex::operator-=(const Hex &hex) {
    this->x -= hex.x;
    this->y -= hex.y;
    this->z -= hex.z;
    return *this;
}

Hex operator-(Hex &a, const Hex &b) {
    return Hex(a.x - b.x, a.y - b.y, a.y - b.y);
}


Hex hex_direction(int direction) {
    return hex_directions[direction];
}

Hex Hex::hex_neighbor(int direction) {
    return *this + this->hex_direction(direction);
}

Hex Hex::hex_diagonal_neighbor(int direction) {
    return *this + hex_diagonals[direction];
}

int Hex::hex_length() {
    return int((abs(this->x) + abs(this->y) + abs(this->z)) / 2);
}

int Hex::hex_distance(Hex hex) {
    return (*this - hex).hex_length();
}
