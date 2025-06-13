#include "WaveManager.hpp"
#include "Components/Tags/EnemyTag.hpp"
#include "Spawner/Spawner.hpp"
#include "base/camera/CameraManager.hpp"
#include <base/assets/AssetManager.hpp>
#include <base/entities/EntityManager.hpp>
#include <random>

WaveManager::WaveManager(Base::EntityManager *entityMan) : _entityMan(entityMan)
{
}

void WaveManager::GenerateWave()
{

  // Init Variables
  int aliveCount = _entityMan->Query<EnemyTag>().size();

  // Setup
  _enemiesToSpawn.clear();
  _wavePoints = (_baseWavePoints * _currentWave) - (3 * aliveCount);
  // Random
  std::random_device rd;
  std::mt19937_64 gen(rd());

  // Create pool of available enemies for this wave
  std::unordered_map<Spawner::EnemyType, EnemySpec> pool = {};
  for (auto &[type, spec] : _enemySpawnInfo)
  {
    if (_currentWave >= spec.unlockWave && spec.cost <= _wavePoints)
    {
      pool[type] = spec;
    }
  }

  // Safety check - if no enemies are available, exit
  if (pool.empty())
  {
    _currentWave++;
    return; // Avoid infinite loop if no enemies can be spawned
  }

  // Calculate total chance for weighted selection
  float totalChance = 0;
  for (auto &[type, spec] : pool)
  {
    totalChance += spec.spawnChance;
  }

  while (_wavePoints > 0)
  {
    bool canAffordAny = false;
    for (auto &[type, spec] : pool)
    {
      if (spec.cost <= _wavePoints)
      {
        canAffordAny = true;
        break;
      }
    }

    if (!canAffordAny || pool.empty())
    {
      break;
    }

    float selectedChance = std::uniform_real_distribution<float>(0, totalChance)(gen);
    float runningSum = 0;
    bool enemySelected = false;

    for (auto &[type, spec] : pool)
    {
      if (spec.cost <= _wavePoints)
      {
        runningSum += spec.spawnChance;
        if (selectedChance <= runningSum)
        {
          _wavePoints -= spec.cost;
          _enemiesToSpawn.push_back(type); // Fixed: removed asterisks
          enemySelected = true;
          break;
        }
      }
    }

    if (!enemySelected)
    {
      Spawner::EnemyType cheapestType;
      int lowestCost = INT_MAX;

      for (auto &[type, spec] : pool)
      {
        if (spec.cost <= _wavePoints && spec.cost < lowestCost)
        {
          lowestCost = spec.cost;
          cheapestType = type;
        }
      }

      if (lowestCost != INT_MAX)
      {
        _wavePoints -= pool[cheapestType].cost;
        _enemiesToSpawn.push_back(cheapestType);
      }
    }
  }

  _spawner.SetToSpawn(_enemiesToSpawn);
  _currentWave++;
}

void WaveManager::SpawnWaves(float dt, Base::CameraManager *camManager, const Base::Scene *currentScene)
{
  int count = _entityMan->Query<EnemyTag>().size();
  if (_waveTimer >= _waveTime || (count == 0 && _spawner.GetToSpawnCount() == 0))
  {
    _waveTimer = 0;
    GenerateWave();
  }
  else
  {
    _waveTimer += dt;
  }
  _spawner.SpawnWave(dt, _entityMan, currentScene, camManager, _playerID);
}

void WaveManager::SpawnPlayer(const Base::Scene *currentScene, Base::CameraManager *camManager)
{
  _playerID = _spawner.SpawnPlayer(_entityMan, currentScene, camManager, {0, 0});
}
