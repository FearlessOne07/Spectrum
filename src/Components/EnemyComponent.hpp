
#pragma once
#include <base/components/Component.hpp>

enum class EnemyType : uint8_t
{
  NONE = 0,
  CHASER,
  SHOOTER,
  KAMAKAZI
};

struct EnemySpec
{
  EnemyType Type = EnemyType::NONE;
  float SpawnChance = 0;
  int Cost = 0;
  int UnlockWave = 0;

  // Components
  int Value = 0;
  float BaseDamage = 0;
  float BaseHealth = 0;
};

struct EnemyComponent : public Base::Component
{
  int value = 0;
  EnemyType type = EnemyType::NONE;
};
