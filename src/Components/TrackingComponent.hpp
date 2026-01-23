#pragma once
#include "base/entities/Entity.hpp"
#include "base/util/Type.hpp"
#include <base/components/Component.hpp>

class TrackingComponent : public Base::Component
{
  Base::EntityID _targetEntityID;
  Base::Vector2 _targetPosition = {0, 0};

public:
  TrackingComponent(const Base::EntityID &entityID) : _targetEntityID(entityID)
  {
  }

  TrackingComponent(const Base::Vector2 &targetPosition) : _targetPosition(targetPosition)
  {
  }

  Base::EntityID GetTargetEntityID() const
  {
    return _targetEntityID;
  }

  Base::Vector2 GetTargetPosition() const
  {
    return _targetPosition;
  }

  void SetTargetEntity(const Base::EntityID &entityId)
  {
    _targetEntityID = entityId;
  }

  void SetTargetPosition(Base::Vector2 position)
  {
    _targetPosition = position;
  }
};
