#include "EntityManager.h"

Entity EntityManager::CreateEntity()
{
	Entity entity(id, "");
	entities.push_back(entity);
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