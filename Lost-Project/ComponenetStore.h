#pragma once
#ifndef COMPONENT_STORE_H
#define COMPONENT_STORE_H

#include <stdio.h>
#include <unordered_map>
#include "Entity.h"
 
template<typename T>
class ComponentStore {
    std::unordered_map<uint8_t, T> data;

public:
    T& add(uint8_t id) { data[id] = T(); return data[id]; }
    bool has(uint8_t id) { return data.count(id) > 0; }
    T& get(uint8_t id) { return data[id]; }
    std::pair<uint8_t, T&> getFirst() { return { data.begin()->first, data.begin()->second }; }
};

#endif
