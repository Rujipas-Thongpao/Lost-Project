#pragma once
#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <stdio.h>
#include <vector>

#include "Entity.h"

class EntityManager
{

public:
	std::vector<Entity> entities;
	uint8_t id = 0;
	void Init();
	Entity& CreateEntity();
	void Cleanup();
};
#endif // !ENTITY_MANAGER_H
