#pragma once

enum class ContentType {
    VEHICLE,
    BASE,
    CATAPULT,
    HARD_REPAIR,
    LIGHT_RAPAIR,
    OBSTACLE,
    SPAWN_POINT,
    EMPTY
};

class Content {
public:
    int id = -1;
    bool is_reacheble = true;
    ContentType content_type = ContentType::EMPTY;

    explicit Content(bool is_reacheble, ContentType content_type, int id = -1);
};
