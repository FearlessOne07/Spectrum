#pragma once
#include "Components/EnemyComponent.hpp"
#include "Ship/Ship.hpp"
#include "Signals/EntityDiedSignal.hpp"
#include "base/entities/Entity.hpp"
#include "base/scenes/SceneLayer.hpp"
#include <base/assets/AssetManager.hpp>
#include <base/entities/EntityManager.hpp>
#include <queue>
#include <random>
#include <vector>

class Spawner
{
private:
  std::random_device _rd;
  std::mt19937_64 _gen;

  // Spawming
  float _spawnDuration = 2.f;
  float _spawnTimer = _spawnDuration;
  float _spawnOffset = -200;
  std::queue<EnemySpec> _toSpawn = {};
  float _difficultyScale = 1;

  // Player
  Base::EntityID _playerID;

  // Layer
  Base::SceneLayer *_parentLayer = nullptr;

  Base::Ref<Base::EntityManager> _entityManager;

  void SpawnLight(std::shared_ptr<EntityDiedSignal> sig);
  void SpawnHealthPack(std::shared_ptr<EntityDiedSignal> sig);

public:
  Spawner() = default;
  void Init(Base::SceneLayer *parentLayer, Base::Ref<Base::EntityManager> entityManager);
  Base::EntityID SpawnPlayer(Base::Vector2 position, const Ship &ship);
  void SpawnWave(float dt, Base::EntityID playerID);
  int GetToSpawnCount() const;
  void SetToSpawn(std::vector<EnemySpec> toSpawn, float difficultyScale);
};
