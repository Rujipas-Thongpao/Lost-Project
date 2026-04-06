#include "game.h"

#include "statSystem.h"
#include "stathelper.h"
#include "Random.h"
using namespace Engine;

void StatSystem::Update(float dt) {
	Game& game = Game::getInstance();
	for (Entity& e : game.entityManager.entities) {
		if (e.isDestroy) continue;
		if (!game.statStore.has(e.id)) continue;

		StatComponent& stat = game.statStore.get(e.id);
		if (!stat.isInit) {

			stat.finalHealth = stat.baseHealth;
			stat.finalSpeed = stat.speed;
			stat.finalAttackSpeed = stat.attackSpeed;
			stat.finalDamage = stat.damage;
			stat.finalRunSpeed = stat.runSpeed;

			stat.isInit = true;
			continue;
		}

		if (!game.statModifierStore.has(e.id)) continue;

		StatModifierComponent& mods = game.statModifierStore.get(e.id);
		StatHelper::ResolveStat(stat, mods);
	}
}


// in WaveSystem or a BuffSystem — call after wave clear
void StatSystem::GiveRandomBuff(uint16_t entityId) {
    if (showBuffUI) return;
    Game& game = Game::getInstance();
    StatModifierComponent& mods = game.statModifierStore.get(entityId);

    std::vector<Modifier> buffPool = {
        {       StatType::Health,          ModifyType::Add,      1.0f,"",  "+1 Max Health"       },
        {      StatType::Health,          ModifyType::Multiply, 1.25f,"", "+25% Max Health"     },
        {      StatType::Damage,          ModifyType::Add,      0.5f,"",  "+0.5 Damage"         },
        {     StatType::Damage,          ModifyType::Multiply, 1.2f, "", "+20% Damage"         },
        {         StatType::Speed,           ModifyType::Add,      0.5f,"",  "+0.5 Move Speed"     },
        {           StatType::RunSpeed,        ModifyType::Multiply, 1.15f,"", "+15% Run Speed"      },
        {        StatType::AttackSpeed,     ModifyType::Multiply, 1.2f, "", "+20% Attack Speed"   },
        {           StatType::DamageReduction, ModifyType::Add,      0.1f,"",  "+10% Damage Resist"  },
        {         StatType::RegenRate,       ModifyType::Add,      0.5f,"",  "+0.5 HP Regen/sec"   },
        {       StatType::Stamina,         ModifyType::Add,      25.0f,"", "+25 Max Stamina"     },
    };

    // pick 3 random unique options for player to choose
    std::vector<int> indices;
    while (indices.size() < 3) {
        int idx = Random::range(0, (int)buffPool.size() - 1);
        // no duplicates
        if (std::find(indices.begin(), indices.end(), idx) == indices.end())
            indices.push_back(idx);
    }

    // store choices for UI to display
    pendingBuffChoices.clear();
    for (int i : indices)
        pendingBuffChoices.push_back(buffPool[i]);

    showBuffUI = true;
}

// call after player picks — apply the chosen buff
void StatSystem::ApplyBuff(uint16_t entityId, int choiceIndex) {
    if (choiceIndex >= pendingBuffChoices.size()) return;

    Game& game = Game::getInstance();
    StatModifierComponent& mods = game.statModifierStore.get(entityId);

    auto& chosen = pendingBuffChoices[choiceIndex];


    StatHelper::AddModifier(mods, {
        chosen.statType,
        chosen.modifyType,
        chosen.value,
        ""
        });

    showBuffUI = false;
    pendingBuffChoices.clear();
}