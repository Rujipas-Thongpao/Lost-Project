#pragma once
#include <stdio.h>
#include <vector>
#include <string>

class StatSystem {
public:
	bool showBuffUI = false;
	vector<Modifier> pendingBuffChoices;
    void Update(float dt);
	void GiveRandomBuff(uint16_t entityId);
	void ApplyBuff(uint16_t entityId, int choiseIndex);
};
