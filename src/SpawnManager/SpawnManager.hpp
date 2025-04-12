#pragma once
#include "base/AssetManager.hpp"
#include "base/EntityManager.hpp"
#include "raylib/raylib.h"
#include <cstddef>

class SpawnManager
{
private:
  float _spawnDuration = 7.f;
  float _spawnTimer = _spawnDuration;

  float _spawnOffset = 200;

public:
  size_t SpawnPlayer(Base::EntityManager *entityManager, Base::AssetManager *assetManager, Vector2 position);
  void SpawnEnemies(float dt, Base::EntityManager *entityManager, size_t playerID);
};
