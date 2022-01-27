#pragma once

enum class ContentType{
    VEHICLE,
    BASE,
    CATAPULT,
    HARD_REPAIR,
    LIGHT_RAPAIR,
    OBSTACLE,
    EMPTY
};

class Content{
public:
    bool is_reacheble;
    ContentType content_type;

    explicit Content(bool is_reacheble, ContentType content_type);
};
