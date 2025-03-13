#pragma once
#include "base/EntityManager.hpp"
#include "raylib/raylib.h"

class SpawnManager
{
private:
public:
  void SpawnPlayer(Base::EntityManager *entityManager, Vector2 position);
};
