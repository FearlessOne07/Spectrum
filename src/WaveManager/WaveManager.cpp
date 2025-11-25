#include "WaveManager.hpp"
#include "Components/EnemyComponent.hpp"
#include "Ship/Ship.hpp"
#include "Spawner/Spawner.hpp"
#include "base/components/ColliderComponent.hpp"
#include "base/entities/Entity.hpp"
#include "base/scenes/SceneLayer.hpp"
#include "base/util/Ref.hpp"
#include <base/assets/AssetManager.hpp>
#include <base/entities/EntityManager.hpp>
#include <base/scenes/SceneLayer.tpp>
#include <random>

void WaveManager::Init(const Base::SceneLayer *parentLayer, Base::Ref<Base::EntityManager> entityMan)
{
  _entityMan = entityMan;
  _parentLayer = parentLayer;
  _spawner.Init(_parentLayer, _entityMan);
}

void WaveManager::GenerateWave()
{
  std::random_device _rd;
  std::mt19937_64 _gen = std::mt19937_64(_rd());

  // Init Variables
  int aliveCount = _entityMan->Query<EnemyComponent>().size();

  // Setup
  _enemiesToSpawn.clear();
  _wavePoints = (_baseWavePoints * _currentWave) - (3 * aliveCount);

  // Create pool of available enemies for this wave
  std::unordered_map<EnemyType, EnemySpec> pool = {};
  for (auto &spec : _enemySpawnInfo)
  {
    if (_currentWave >= spec.UnlockWave && spec.Cost <= _wavePoints)
    {
      pool[spec.Type] = spec;
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
    totalChance += spec.SpawnChance;
  }

  while (_wavePoints > 0)
  {
    bool canAffordAny = false;
    for (auto &[type, spec] : pool)
    {
      if (spec.Cost <= _wavePoints)
      {
        canAffordAny = true;
        break;
      }
    }

    if (!canAffordAny || pool.empty())
    {
      break;
    }

    float selectedChance = std::uniform_real_distribution<float>(0, totalChance)(_gen);
    float runningSum = 0;
    bool enemySelected = false;

    for (auto &[type, spec] : pool)
    {
      if (spec.Cost <= _wavePoints)
      {
        runningSum += spec.SpawnChance;
        if (selectedChance <= runningSum)
        {
          _wavePoints -= spec.Cost;
          _enemiesToSpawn.push_back(spec);
          enemySelected = true;
          break;
        }
      }
    }

    if (!enemySelected)
    {
      EnemySpec cheapest;
      int lowestCost = INT_MAX;

      for (auto &[type, spec] : pool)
      {
        if (spec.Cost <= _wavePoints && spec.Cost < lowestCost)
        {
          lowestCost = spec.Cost;
          cheapest = spec;
        }
      }

      if (lowestCost != INT_MAX)
      {
        _wavePoints -= cheapest.Cost;
        _enemiesToSpawn.push_back(cheapest);
      }
    }
  }

  if (_currentWave % _difficultyIncrement == 0)
  {
    _difficulty = pow(std::uniform_real_distribution<float>(1.05, 1.1)(_gen), _currentWave / _difficultyIncrement);
  }

  _spawner.SetToSpawn(_enemiesToSpawn, _difficulty);
  _currentWave++;
}

void WaveManager::SpawnWaves(float dt)
{
  int count = _entityMan->Query<EnemyComponent>().size();
  if (_waveTimer >= _waveTime || (count == 0 && _spawner.GetToSpawnCount() == 0))
  {
    _waveTimer = 0;
    GenerateWave();
  }
  else
  {
    _waveTimer += dt;
  }
  _spawner.SpawnWave(dt, _playerID);
}

Base::EntityID WaveManager::SpawnPlayer(const Ship &ship)
{
  _playerID = _spawner.SpawnPlayer({0, 0}, ship);
  return _playerID;
}

WaveManager::~WaveManager()
{
}
