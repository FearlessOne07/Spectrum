#pragma once
#include "base/entities/Entity.hpp"
#include "raylib.h"
#include <base/components/Component.hpp>

class TrackingComponent : public Base::Component
{
  Base::EntityID _targetEntityID;
  Vector2 _targetPosition = {0, 0};

public:
  TrackingComponent(const Base::EntityID &entityID) : _targetEntityID(entityID)
  {
  }

  TrackingComponent(const Vector2 &targetPosition) : _targetPosition(targetPosition)
  {
  }

  Base::EntityID GetTargetEntityID() const
  {
    return _targetEntityID;
  }

  Vector2 GetTargetPosition() const
  {
    return _targetPosition;
  }

  void SetTargetEntity(const Base::EntityID &entityId)
  {
    _targetEntityID = entityId;
  }

  void SetTargetPosition(Vector2 position)
  {
    _targetPosition = position;
  }
};
