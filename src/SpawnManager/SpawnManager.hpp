#pragma once
#include "base/EntityManager.hpp"
#include "raylib/raylib.h"
#include <cstddef>

class SpawnManager
{
private:
public:
  size_t SpawnPlayer(Base::EntityManager *entityManager, Vector2 position);
};
