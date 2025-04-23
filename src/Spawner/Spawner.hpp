#pragma once
#include "base/AssetManager.hpp"
#include "base/EntityManager.hpp"
#include "raylib/raylib.h"
#include <cstddef>
#include <queue>
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

public:
  size_t SpawnPlayer(Base::EntityManager *entityManager, Base::AssetManager *assetManager, Vector2 position);
  void SpawnEnemies(float dt, Base::EntityManager *entityManager, size_t playerID);
  void SpawnWave(float dt, Base::EntityManager *entityManager, size_t playerID);

  void SetToSpawn(std::vector<EnemyType> toSpawn);
  int GetToSpawnCount() const;
};
