#pragma once
#ifndef TAG_STORE_H
#define TAG_STORE_H

#include <unordered_map>
#include <vector>
#include <stdint.h>
#include "Tags.h"

class TagStore {
    std::unordered_map<Tag, std::vector<uint8_t>> tagToEntities;
    std::unordered_map<uint8_t, std::vector<Tag>> entityToTags;

public:
    void add(uint8_t id, Tag tag) {
        tagToEntities[tag].push_back(id);
        entityToTags[id].push_back(tag);
    }

    void remove(uint8_t id, Tag tag) {
        auto& entities = tagToEntities[tag];
        entities.erase(std::remove(entities.begin(), entities.end(), id), entities.end());

        auto& tags = entityToTags[id];
        tags.erase(std::remove(tags.begin(), tags.end(), tag), tags.end());
    }

    uint8_t& getEntity(Tag tag) {
        return tagToEntities[tag][0];
    }
    // get all entities with a tag
    std::vector<uint8_t>& getEntities(Tag tag) {
        return tagToEntities[tag];
    }

    // get all tags on an entity
    std::vector<Tag>& getTags(uint8_t id) {
        return entityToTags[id];
    }

    bool has(uint8_t id, Tag tag) {
        auto it = entityToTags.find(id);
        if (it == entityToTags.end()) return false;
        auto& tags = it->second;
        return std::find(tags.begin(), tags.end(), tag) != tags.end();
    }
};

#endif // !TAG_STORE_H
