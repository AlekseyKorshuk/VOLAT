#include "Position.h"

const static std::vector<Position> position_directions =
        {Position(1, 0, -1), Position(1, -1, 0), Position(0, -1, 1), Position(-1, 0, 1), Position(-1, 1, 0),
         Position(0, 1, -1)};


Position::Position(int x, int y, int z) : x_(x), y_(y), z_(z) {}

Position::Position(const Position &pos) : x_(pos.x_), y_(pos.y_), z_(pos.z_) {}

int Position::getX() const {
    return x_;
}

int Position::getY() const {
    return y_;
}

int Position::getZ() const {
    return z_;
}

int Position::getLength() const {
    return (abs(x_) + abs(y_) + abs(z_)) / 2;
}

int Position::getDistance(const Position &pos) const {
    return (*this - pos).getLength();
}

bool Position::operator<(const Position &other) const {
    return std::tie(x_, y_, z_) < std::tie(other.x_, other.y_, other.z_);
}


Position operator+(const Position &a, const Position &b) {
    return Position(a.x_ + b.x_, a.y_ + b.y_, a.z_ + b.z_);
}

Position &Position::operator+=(const Position &pos) {
    this->x_ += pos.x_;
    this->y_ += pos.y_;
    this->z_ += pos.z_;
    return *this;
}

Position operator-(const Position &a, const Position &b) {
    return Position(a.x_ - b.x_, a.y_ - b.y_, a.z_ - b.z_);
}

Position &Position::operator-=(const Position &pos) {
    this->x_ -= pos.x_;
    this->y_ -= pos.y_;
    this->z_ -= pos.z_;
    return *this;
}

Position Position::getNeighbor(int direction) {
    return *this + position_directions[direction];
}


bool Position::operator==(const Position &pos) const {
    return x_ == pos.getX() && y_ == pos.getY() && z_ == pos.getZ();
}

bool Position::operator==(Position &pos) const {
    return x_ == pos.getX() && y_ == pos.getY() && z_ == pos.getZ();
}

bool Position::operator!=(Position &pos) const {
    return !(*this == pos);
}

