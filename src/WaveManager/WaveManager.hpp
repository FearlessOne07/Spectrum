#pragma once
#include "Spawner/Spawner.hpp"
#include <base/assets/AssetManager.hpp>
#include <base/entities/EntityManager.hpp>
#include <cstddef>
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
  };

private:
  void GenerateWave();

private:
  // Enemy Weights
  std::unordered_map<Spawner::EnemyType, EnemySpec> _enemySpawnInfo = {
      {Spawner::EnemyType::CHASER, EnemySpec{.spawnChance = 0.8f, .cost = 1, .unlockWave = 1}},
      {Spawner::EnemyType::SHOOTER, EnemySpec{.spawnChance = 0.2f, .cost = 2, .unlockWave = 2}},
  };

  // Wave
  unsigned _currentWave = 1;
  int _wavePoints = 0;
  int _baseWavePoints = 5;
  float _waveTime = 20;
  float _waveTimer = _waveTime;

  // EntityManager
  Base::EntityManager *_entityMan = nullptr;

  // Spawner
  Spawner _spawner = Spawner();

  // Enemies
  std::vector<Spawner::EnemyType> _enemiesToSpawn = {};

  // Player
  size_t _playerID = 0;

public:
  WaveManager() = default;
  WaveManager(Base::EntityManager *entityManager);
  void SpawnWaves(float dt);
  void SpawnPlayer(Base::AssetManager *assetManager);
};
