#pragma once

#include <cmath>
#include <tuple>
#include <ostream>
#include <vector>

class Position {
public:
    Position() {
        x_= -100;
        y_= -100;
        z_= -100;
    }

    Position(int x, int y, int z);

    Position(const Position &pos);

    int getX() const;

    int getY() const;

    int getZ() const;

    int getLength() const;

    int getDistance(const Position &pos) const;

    bool operator<(const Position &other) const;

    Position getNeighbor(int direction);

    friend Position operator+(const Position &a, const Position &b);

    Position &operator+=(const Position &pos);

    friend Position operator-(const Position &a, const Position &b);

    Position &operator-=(const Position &pos);


    bool operator==(const Position &pos) const;

    bool operator==(Position &pos) const;

    bool operator!=(Position &pos) const;


    friend std::ostream &operator<<(std::ostream &stream, const Position &pos) {
        stream << "(" << pos.getX() << "," << pos.getY() << "," << pos.getZ() << ")";
        return stream;
    }


private:
    int x_ = 0;
    int y_ = 0;
    int z_ = 0;
};
