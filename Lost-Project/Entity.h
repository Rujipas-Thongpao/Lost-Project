#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include <stdio.h>
#include <stdint.h>
#include <string>

class Entity
{
public:
	uint16_t id;
	std::string name;
	bool isDestroy;
	Entity(uint16_t id, std::string name) : id(id), name(name), isDestroy(false) {}
};
#endif // !ENTITY_H
