#pragma once
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
