#pragma once
#include "base/entities/Entity.hpp"
#include "raylib.h"
#include <base/components/Component.hpp>

struct TrackingComponent : public Base::Component
{
  bool isTracking = true;
  Base::EntityID targetEntityID = Base::EntityID(-1);
  float trackingDistance = 0.f;
  Vector2 targetDirection = {0, 0};
};
