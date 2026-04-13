#pragma once
#include <vector>

enum class StatType {
    Health, Damage, Speed,
    RunSpeed, AttackSpeed,
    DamageReduction, RegenRate, Stamina, BulletCount
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
    bool isInit = false;
    float baseHealth = 10.0f;
    float damageReduction = 0.0f;
    float regenHealthRate = 0.0f;
    float damage = 3.0f;
    float attackSpeed = 1.0f;
    float speed = 10.0f;
    float stamina = 10.0f;
	float currentStamina = 10.0f;
    float runSpeed = 15.0f;
    int bulletCount = 1;

	// resolved stats after applying modifiers
    float finalHealth;
    float finalDamage;
    float finalSpeed;
	float finalRunSpeed;
	float finalAttackSpeed; // round/seconds
	int finalBulletCount;
};

// This component holds all the modifiers that can affect the stats of an entity.
struct StatModifierComponent {
    std::vector<Modifier> modifiers;
    bool isDirty = false;
};