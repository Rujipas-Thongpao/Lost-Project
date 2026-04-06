#pragma once
#include <vector>

struct Wave {
	int enemyCount;
};

class WaveSystem {
public:
	std::vector<Wave> waves = {
		{ 1},   
		//{ 8},   
		//{ 10},   
		//{ 15},  
		//{ 20}, 
	};
	int currentWave;
	int maxEnemyCountInThisWave;
	void Init();
	void Update(float dt);
	int GetEnemyLeftInThisWaveCount();
	void SpawnEnemies(int wave);
	void NextWave();
};
