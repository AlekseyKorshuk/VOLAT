#pragma once

#include <cmath>
#include <tuple>
#include <ostream>
#include <vector>

/**
 * Position class
 */
class Position {
public:
    /**
     * Position constructor
     */
    Position() {
        x_ = -100;
        y_ = -100;
        z_ = -100;
    }

    /**
     * Position constructor
     * @param x Coordinate X
     * @param y Coordinate Y
     * @param z Coordinate Z
     */
    Position(int x, int y, int z);

    /**
     * Position constructor
     * @param pos Position
     */
    Position(const Position &pos);

    /**
     * Method to get coordinate
     * @return Coordinate
     */
    int getX() const;

    /**
     * Method to get coordinate
     * @return Coordinate
     */
    int getY() const;

    /**
     * Method to get coordinate
     * @return Coordinate
     */
    int getZ() const;

    /**
     * Method to get length
     * @return length
     */
    int getLength() const;

    /**
     * Method to get distance
     * @param pos Position
     * @return Distance
     */
    int getDistance(const Position &pos) const;

    /**
     * Comparator
     * @param other Position
     * @return Boolean result
     */
    bool operator<(const Position &other) const;

    /**
     * Method to get neighbor
     * @param direction Direction
     * @return Position
     */
    Position getNeighbor(int direction);

    /**
     * Operator +
     * @param a Position
     * @param b Position
     * @return Position
     */
    friend Position operator+(const Position &a, const Position &b);

    /**
     * Operator +=
     * @param pos Position
     * @return Position
     */
    Position &operator+=(const Position &pos);

    /**
     * Operator -
     * @param a Position
     * @param b Position
     * @return Position
     */
    friend Position operator-(const Position &a, const Position &b);

    /**
     * Operator -=
     * @param pos Position
     * @return Position
     */
    Position &operator-=(const Position &pos);

    /**
     * Operator ==
     * @param pos Position
     * @return Boolean result
     */
    bool operator==(const Position &pos) const;

    /**
     * Operator ==
     * @param pos Position
     * @return Boolean result
     */
    bool operator==(Position &pos) const;

    /**
     * Operator !=
     * @param pos Position
     * @return Boolean result
     */
    bool operator!=(Position &pos) const;

    /**
     * Ostream
     * @param stream Stream
     * @param pos Position
     * @return Ostream
     */
    friend std::ostream &operator<<(std::ostream &stream, const Position &pos) {
        stream << "(" << pos.getX() << "," << pos.getY() << "," << pos.getZ() << ")";
        return stream;
    }


private:
    int x_ = 0; // Coordinate X
    int y_ = 0; // Coordinate Y
    int z_ = 0; // Coordinate Z
};
