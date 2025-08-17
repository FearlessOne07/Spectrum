#pragma once
#include "base/entities/Entity.hpp"
#include <base/components/Component.hpp>

class TrackingComponent : public Base::Component
{
  Base::EntityID _targetEntityID;

public:
  TrackingComponent(const Base::EntityID &entityID) : _targetEntityID(entityID)
  {
  }

  Base::EntityID GetTargetEntityID() const
  {
    return _targetEntityID;
  }
};
