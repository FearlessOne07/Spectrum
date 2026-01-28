#pragma once
#include "Components/EnemyComponent.hpp"
#include <unordered_map>
class EnemySpecDataBase
{
private:
  inline static std::unordered_map<EnemyType, EnemySpec> _specs = {
    {
      EnemyType::Chaser,
      EnemySpec{
        .SpawnChance = 0.7f,
        .Cost = 1,
        .UnlockWave = 1,
        .Value = 2,
        .BaseDamage = 1,
        .BaseHealth = 3,
      },
    },
    {

      EnemyType::Shooter,
      EnemySpec{
        .SpawnChance = 0.2f,
        .Cost = 2,
        .UnlockWave = 2,
        .Value = 3,
        .BaseDamage = 2,
        .BaseHealth = 3,
      },
    },
    {
      EnemyType::Kamikaze,
      EnemySpec{
        .SpawnChance = 0.1f,
        .Cost = 3,
        .UnlockWave = 4,
        .Value = 5,
        .BaseDamage = 5,
        .BaseHealth = 4,
      },
    },
  };

public:
  static const EnemySpec &GetSpec(const EnemyType &type)
  {
    return _specs.at(type);
  }
};

struct SpawnSpec
{
  EnemyType Type;
  bool IsHealthPacker = false;
};
