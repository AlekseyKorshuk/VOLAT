#include "Player.h"


Player::Player(json state) {
    id = state["idx"].get<std::int32_t>();
    name = state["name"].get<std::string>();
}