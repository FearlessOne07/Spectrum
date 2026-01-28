#pragma once
#include "Components/EnemyComponent.hpp"
#include "Ship/Ship.hpp"
#include "Spawner/Spawner.hpp"
#include "base/entities/Entity.hpp"
#include "base/scenes/SceneLayer.hpp"
#include "base/util/Ref.hpp"
#include <array>
#include <base/assets/AssetManager.hpp>
#include <base/entities/EntityManager.hpp>
#include <vector>

class WaveManager
{
private:
  void GenerateWave();

private:
  // Types
  std::array<EnemyType, 3> _enemyTypes = {EnemyType::Kamikaze, EnemyType::Shooter, EnemyType::Chaser};

  // Wave
  unsigned _currentWave = 1;
  int _wavePoints = 0;
  int _baseWavePoints = 5;
  float _waveTime = 45;
  float _waveTimer = _waveTime;
  float _difficulty = 1;
  int _difficultyIncrement = 5;

  float _healthPackerProbabilty = 0.05;

  // EntityManager
  Base::Ref<Base::EntityManager> _entityMan;

  // Spawner
  Spawner _spawner = Spawner();

  // Enemies
  std::vector<SpawnSpec> _enemiesToSpawn = {};

  // Player
  Base::EntityID _playerID;

  // Layer
  Base::SceneLayer *_parentLayer = nullptr;

public:
  WaveManager() = default;
  void Init(Base::SceneLayer *parentLayer, Base::Ref<Base::EntityManager> entityManager);
  Base::EntityID SpawnPlayer(const Ship &ship);
  void SpawnWaves(float dt);
  ~WaveManager();
};
