#include "Hex.h"
#include "../content/vehicles/MediumTank.h"

#include <tuple>

Hex::Hex(int x, int y, int z, ContentType content_type, json data, int id) : pos(x, y, z) {
    this->setHex(content_type, data, id);
    danger = std::vector<double>(5, 0);
}

void Hex::addNeighbour(Position pos) {
    this->neighbors.push_back(pos);
}

json Hex::getJson() {
    json j;
    j["x"] = this->pos.getX();
    j["y"] = this->pos.getY();
    j["z"] = this->pos.getZ();
    return j;
}

void Hex::setHex(ContentType content_type, json data, int id) {
    this->is_occupied = true;
    switch (content_type) {
        case ContentType::BASE: {
            this->is_occupied = false;
            this->content = std::make_shared<Content>(true, ContentType::BASE);
            break;
        }
        case ContentType::SPAWN_POINT: {
            this->is_occupied = false;
            this->content = std::make_shared<Content>(false, ContentType::SPAWN_POINT);
            break;
        }
        case ContentType::OBSTACLE: {
            this->is_occupied = false;
            this->content = std::make_shared<Content>(false, ContentType::OBSTACLE);
            break;
        }
        default: {
            this->is_occupied = false;
            this->content = std::make_shared<Content>(true, ContentType::EMPTY);
            break;
        }
    }
}

void Hex::clear() {
    is_occupied = false;
    content.reset();
    content = nullptr;
    visited = false;
    prev = {-100, -100, -100};
}