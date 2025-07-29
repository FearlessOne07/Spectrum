#pragma once

#include "base/entities/Entity.hpp"
#include <base/components/Component.hpp>
#include <raylib.h>

struct TransformEffectsComponent : public Base::Component
{
  // Rotation
  Base::EntityID lookAtTarget;
  bool rotate = true;
  bool lookAt = false;
  float lookAtDistance = 0.f;
  float targetAngularVelocity = 1; // Deg per Second
  float angularVelocity = 0;
  float angularAcceleration = 0;

  // Bounds
  bool bind = false;
  Vector2 bindMin = {0, 0};
  Vector2 bindMax = {0, 0};
};
