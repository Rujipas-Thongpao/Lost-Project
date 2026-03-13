#include "EntityManager.h"
#include <stdio.h>
#include <iostream>

Entity& EntityManager::CreateEntity()
{
	Entity entity(id, "");
	entities.push_back(entity);
	std::cout << "Create entity : " << (unsigned)entity.id << ", " << entity.name << std::endl;
	id++;
	return entity;
}

void EntityManager::Init()
{
	entities.clear();
	id = 0;
}

void EntityManager::Cleanup()
{
	entities.clear();
	id = 0;
}