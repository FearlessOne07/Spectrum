#pragma once
#include "base/renderer/RenderLayer.hpp"
#include "base/scenes/Scene.hpp"
#include "base/scenes/SceneLayer.hpp"
#include <base/assets/AssetManager.hpp>
#include <base/entities/EntityManager.hpp>
#include <cstddef>
#include <queue>
#include <raylib.h>
#include <vector>

class Spawner
{
public:
  enum class EnemyType : uint8_t
  {
    CHASER = 0,
    SHOOTER
  };

private:
  // Spawming
  float _spawnDuration = 2.f;
  float _spawnTimer = _spawnDuration;
  float _spawnOffset = 200;
  std::queue<EnemyType> _toSpawn = {};

  // Player
  size_t _playerID = -1;

  // Layer
  const Base::SceneLayer *_parentLayer = nullptr;

public:
  Spawner() = default;
  Spawner(const Base::SceneLayer *parentLayer);
  size_t SpawnPlayer( //
    Base::EntityManager *entityManager,
    Vector2 position //
  );
  void SpawnWave( //
    float dt, Base::EntityManager *entityManager,
    size_t playerID //
  );
  int GetToSpawnCount() const;
  void SetToSpawn(std::vector<EnemyType> toSpawn);
};
