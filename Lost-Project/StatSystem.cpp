#include "game.h"

#include "statSystem.h"
#include "stathelper.h"

void StatSystem::Update(float dt) {
	Game& game = Game::getInstance();
	for (Entity& e : game.entityManager.entities) {
		if (e.isDestroy) continue;
		if (!game.statStore.has(e.id)) continue;
		if (!game.statModifierStore.has(e.id)) continue;

		StatComponent& stat = game.statStore.get(e.id);
		StatModifierComponent& mods = game.statModifierStore.get(e.id);

		StatHelper::ResolveStat(stat, mods);
	}
}