#pragma once
#include "StatComponent.h"
#include <string>
class StatHelper {
public:
    static void ResolveStat(StatComponent& stat, StatModifierComponent& mods) {
        if (!mods.isDirty) return;  // skip if nothing changed

        // start from base values
        float health = stat.baseHealth;
        float damage = stat.damage;
        float speed = stat.speed;
        float runSpeed = stat.runSpeed;
        float attackSpeed = stat.attackSpeed;

        // apply flat additions first
        for (auto& m : mods.modifiers) {
            if (m.modifyType != ModifyType::Add) continue;
            switch (m.statType) {
            case StatType::Health:      health += m.value; break;
            case StatType::Damage:      damage += m.value; break;
            case StatType::Speed:       speed += m.value; break;
            case StatType::RunSpeed:    runSpeed += m.value; break;
            case StatType::AttackSpeed: attackSpeed += m.value; break;
            }
        }

        // then multipliers
        for (auto& m : mods.modifiers) {
            if (m.modifyType != ModifyType::Multiply) continue;
            switch (m.statType) {
            case StatType::Health:  health *= m.value; break;
            case StatType::Damage:  damage *= m.value; break;
            case StatType::Speed:   speed *= m.value; break;
            }
        }

        // write to final stats
        stat.finalHealth = health;
        stat.finalDamage = damage;
        stat.finalSpeed = speed;
        stat.finalRunSpeed = runSpeed;
        stat.finalAttackSpeed = attackSpeed;

        mods.isDirty = false;  // resolved, no need to recalculate next frame
    }

    // helper to add modifier and mark dirty
    static void AddModifier(StatModifierComponent& mods, Modifier mod) {
        mods.modifiers.push_back(mod);
        mods.isDirty = true;  // trigger recalculate
    }

    // remove by source name — when item unequipped or buff expires
    static void RemoveModifier(StatModifierComponent& mods,
        const std::string& source) {
        mods.modifiers.erase(
            std::remove_if(mods.modifiers.begin(), mods.modifiers.end(),
                [&](const Modifier& m) { return m.source == source; }),
            mods.modifiers.end()
        );
        mods.isDirty = true;
    }
};
