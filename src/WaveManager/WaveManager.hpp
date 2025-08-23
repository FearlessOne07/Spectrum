#pragma once
#include "Signals/EntityDiedSignal.hpp"
#include "Spawner/Spawner.hpp"
#include "base/entities/Entity.hpp"
#include "base/scenes/SceneLayer.hpp"
#include <base/assets/AssetManager.hpp>
#include <base/entities/EntityManager.hpp>
#include <memory>
#include <unordered_map>
#include <vector>

class WaveManager
{
public:
  struct EnemySpec
  {
    float spawnChance = 0;
    int cost = 0;
    int unlockWave = 0;
    int value = 0;
  };

private:
  void GenerateWave();
  void SpawnLight(std::shared_ptr<EntityDiedSignal> sig);

private:
  // Enemy Weights
  std::unordered_map<EnemyType, EnemySpec> _enemySpawnInfo = {
    {EnemyType::CHASER, EnemySpec{.spawnChance = 0.8f, .cost = 1, .unlockWave = 1, .value = 2}},
    {EnemyType::SHOOTER, EnemySpec{.spawnChance = 0.2f, .cost = 2, .unlockWave = 2}},
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
  std::vector<EnemyType> _enemiesToSpawn = {};

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
