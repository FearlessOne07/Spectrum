#pragma once
#include "Components/EnemyComponent.hpp"
#include "Ship/Ship.hpp"
#include "Signals/EntityDiedSignal.hpp"
#include "base/entities/Entity.hpp"
#include "base/scenes/Scene.hpp"
#include "base/scenes/SceneLayer.hpp"
#include <base/assets/AssetManager.hpp>
#include <base/entities/EntityManager.hpp>
#include <queue>
#include <raylib.h>
#include <vector>

class Spawner
{
private:
  // Spawming
  float _spawnDuration = 2.f;
  float _spawnTimer = _spawnDuration;
  float _spawnOffset = -200;
  std::queue<EnemySpec> _toSpawn = {};
  float _difficultyScale = 1;

  // Player
  Base::EntityID _playerID;

  // Layer
  const Base::SceneLayer *_parentLayer = nullptr;

  Base::Ref<Base::EntityManager> _entityManager;

  void SpawnLight(std::shared_ptr<EntityDiedSignal> sig);

public:
  Spawner() = default;
  void Init(const Base::SceneLayer *parentLayer, Base::Ref<Base::EntityManager> entityManager);
  Base::EntityID SpawnPlayer(Vector2 position, const Ship &ship);
  void SpawnWave(float dt, Base::EntityID playerID);
  int GetToSpawnCount() const;
  void SetToSpawn(std::vector<EnemySpec> toSpawn, float difficultyScale);
};
