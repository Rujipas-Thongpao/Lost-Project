#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include <stdio.h>
#include <stdint.h>
#include <string>

class Entity
{
public:
	uint8_t id;
	std::string name;
	Entity(uint8_t id, std::string name) : id(id), name(name) {}
};
#endif // !ENTITY_H
