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
	void add(uint8_t id) { data[id] = T(); }
    bool has(uint8_t id) { return data.count(id) > 0; }
    T& get(uint8_t id) { return data[id]; }
};

#endif
