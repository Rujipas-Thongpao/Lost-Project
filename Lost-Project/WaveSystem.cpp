#include "WaveSystem.h"
#include "game.h"
#include "StatHelper.h"

void WaveSystem::Init() {
	currentWave = 0;
	SpawnEnemies(currentWave);
}

int WaveSystem::GetEnemyLeftInThisWaveCount() {
	Game& game = Game::getInstance();
	std::vector<uint16_t> enemies = game.tagStore.getEntities(Tag::Enemy);
	int left = 0;
	for (uint16_t e : enemies) {
		if (!game.entityManager.entities[e].isDestroy) left++;
	}
	return left;
}

void WaveSystem::Update(float dt) {
	Game& game = Game::getInstance();
	uint16_t player = game.tagStore.getEntity(Tag::Player);
	if (GetEnemyLeftInThisWaveCount() == 0) {
		game.statSystem.GiveRandomBuff(player);
		//currentWave++;
		//SpawnEnemies(currentWave);
        // equip sword — adds flat +2 damage
		//StatModifierComponent& mods = game.statModifierStore.get(player);
  //      StatHelper::AddModifier(mods, {
  //          StatType::Damage, ModifyType::Add, 2.0f, "sword"
  //          });
	}
}

void WaveSystem::NextWave() {
	currentWave++;
	SpawnEnemies(currentWave);
}

void WaveSystem::SpawnEnemies(int wave) {
	Game& game = Game::getInstance();
	uint8_t block_meshId = game.assetManager.GetModelData("cat_mesh");

	int maxWave = waves.size();
	int enemyCount = 0;

	if(wave < 0 || wave >= maxWave) {
		enemyCount = waves[maxWave - 1].enemyCount * (wave-maxWave);
	}
	else {
		enemyCount = waves[wave].enemyCount;
	}

	maxEnemyCountInThisWave = enemyCount;

	for (int i = 0; i < enemyCount; i++) {
		uint16_t enemy = game.entityManager.CreateEntity().id;
		game.tagStore.add(enemy, Tag::Enemy);

		game.enemyStore.add(enemy);
		game.statStore.add(enemy);
		game.healthStore.add(enemy);

		MeshComponent& enemy_mesh = game.meshStore.add(enemy);
		enemy_mesh.mesh_id = block_meshId;

		TransformComponent& enemy_tf = game.transformStore.add(enemy);
		enemy_tf.position = glm::vec3(5.0f, 0.0f, 3.0f*i);

		ColliderComponent& enemy_col = game.colliderStore.add(enemy);
		enemy_col.size = glm::vec3(1, 1, 1);
		enemy_col.isStatic = false;

		MaterialComponent& block_mat = game.materialStore.add(enemy);
		float rx = (float)rand() / RAND_MAX;
		float rz = (float)rand() / RAND_MAX;
		float ra = (float)rand() / RAND_MAX;
		enemy_tf.position = glm::vec3(20.0f * rx, 0.0f, 20.0f * rz);
		enemy_tf.rotation = glm::vec3(0, 360.f * ra, 0);
	}
}
