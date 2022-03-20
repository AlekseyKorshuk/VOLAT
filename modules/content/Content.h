#pragma once

/**
 * Enumerator of possible objects
 */
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

/**
 * Base Content class
 */
class Content {
public:
    int id = -1;                                    // Object id
    bool is_reacheble = true;                       // Whenever object is reachable
    ContentType content_type = ContentType::EMPTY;  // Content type

    /**
     * Content constructor
     * @param is_reacheble  Whenever object is reachable
     * @param content_type  Content type
     * @param id            Object id
     */
    explicit Content(bool is_reacheble, ContentType content_type, int id = -1);
};
