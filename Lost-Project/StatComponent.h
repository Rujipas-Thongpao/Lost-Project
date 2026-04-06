#pragma once
#include <vector>

enum class StatType {
    Health, Damage, Speed,
    RunSpeed, AttackSpeed,
    DamageReduction, RegenRate, Stamina
};

enum class ModifyType {
    Add,        
    Subtract,   
    Multiply,  
    Override  
};

struct Modifier {
    StatType   statType;
    ModifyType modifyType;
    float      value;
    std::string source;  
	std::string description;
};

struct StatComponent {
    float baseHealth = 10.0f;
    float damageReduction = 0.0f;
    float regenHealthRate = 0.0f;
    float damage = 3.0f;
    float attackSpeed = 1.0f;
    float speed = 10.0f;
    float stamina = 10.0f;
    float runSpeed = 15.0f;

	float currentStamina = 10.0f;

	// resolved stats after applying modifiers
    float finalHealth = 10.0f;
    float finalDamage = 3.0f;
    float finalSpeed = 10.0f;
	float finalRunSpeed = 15.0f;
	float finalAttackSpeed = 1.0f; // round/seconds
};

// This component holds all the modifiers that can affect the stats of an entity.
struct StatModifierComponent {
    std::vector<Modifier> modifiers;
    bool isDirty = false;
};