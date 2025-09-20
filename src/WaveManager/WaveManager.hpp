#pragma once
#include "Signals/EntityDiedSignal.hpp"
#include "Spawner/Spawner.hpp"
#include "base/entities/Entity.hpp"
#include "base/scenes/SceneLayer.hpp"
#include <base/assets/AssetManager.hpp>
#include <base/entities/EntityManager.hpp>
#include <memory>
#include <vector>

class WaveManager
{
public:
private:
  void GenerateWave();

private:
  // Enemy Weights
  std::vector<EnemySpec> _enemySpawnInfo = {
    EnemySpec{
      .Type = EnemyType::CHASER,
      .SpawnChance = 0.7f,
      .Cost = 1,
      .UnlockWave = 1,
      .Value = 2,
      .BaseDamage = 1.5,
      .BaseHealth = 4,
    },
    EnemySpec{
      .Type = EnemyType::SHOOTER,
      .SpawnChance = 0.2f,
      .Cost = 2,
      .UnlockWave = 2,
      .Value = 3,
      .BaseDamage = 1,
      .BaseHealth = 4,
    },
    EnemySpec{
      .Type = EnemyType::KAMIKAZE,
      .SpawnChance = 0.1f,
      .Cost = 3,
      .UnlockWave = 3,
      .Value = 5,
      .BaseDamage = 5,
      .BaseHealth = 4,
    },
  };

  // Wave
  unsigned _currentWave = 1;
  int _wavePoints = 0;
  int _baseWavePoints = 5;
  float _waveTime = 45;
  float _waveTimer = _waveTime;

  // Random

  // EntityManager
  Base::EntityManager *_entityMan = nullptr;

  // Spawner
  Spawner _spawner = Spawner();

  // Enemies
  std::vector<EnemySpec> _enemiesToSpawn = {};

  // Player
  Base::EntityID _playerID;

  // Layer
  const Base::SceneLayer *_parentLayer = nullptr;

public:
  WaveManager() = default;
  void Init(const Base::SceneLayer *parentLayer, Base::EntityManager *entityManager);
  Base::EntityID SpawnPlayer();
  ~WaveManager();
  void SpawnWaves(float dt);
};
