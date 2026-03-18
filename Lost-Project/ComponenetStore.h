#pragma once
#ifndef COMPONENT_STORE_H
#define COMPONENT_STORE_H

#include <stdio.h>
#include <unordered_map>
#include "Entity.h"
 
template<typename T>
class ComponentStore {
    std::unordered_map<uint16_t, T> data;

public:
    T& add(uint16_t id) { data[id] = T(); return data[id]; }
    bool has(uint16_t id) { return data.count(id) > 0; }
    T& get(uint16_t id) { return data[id]; }
    std::pair<uint16_t, T&> getFirst() { return { data.begin()->first, data.begin()->second }; }
};

#endif
