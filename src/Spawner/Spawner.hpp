#pragma once
#include "Components/EnemyComponent.hpp"
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

  // Player
  Base::EntityID _playerID;

  // Layer
  const Base::SceneLayer *_parentLayer = nullptr;

public:
  Spawner() = default;
  Spawner(const Base::SceneLayer *parentLayer);
  Base::EntityID SpawnPlayer(Base::EntityManager *entityManager, Vector2 position);
  void SpawnWave(float dt, Base::EntityManager *entityManager, Base::EntityID playerID);
  int GetToSpawnCount() const;
  void SetToSpawn(std::vector<EnemySpec> toSpawn);
};
