#pragma once

#include "base/entities/Entity.hpp"
#include "base/util/Type.hpp"
#include <base/components/Component.hpp>

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
  Base::Vector2 bindMin = {0, 0};
  Base::Vector2 bindMax = {0, 0};
};
