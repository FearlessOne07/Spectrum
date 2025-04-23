#pragma once

#include "raylib/raylib.h"
#include <base/Component.hpp>

struct TransformEffectsComponent : public Base::Component
{
  // Rotation
  bool rotate = true;
  float targetAngularVelocity = 1; // Deg per Second
  float angularVelocity = 0;
  float angularAcceleration = 0;

  // Bounds
  bool bind = false;
  Vector2 bindMin = {0, 0};
  Vector2 bindMax = {0, 0};
};
