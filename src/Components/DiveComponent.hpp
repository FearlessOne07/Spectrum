#pragma once
#include "base/components/Component.hpp"
#include "base/entities/Entity.hpp"
class DiveComponent : public Base::Component
{
private:
  Base::EntityID _diveTarget;
  bool _hasDived = false;

public:
  DiveComponent(Base::EntityID diveTarget) : _diveTarget(diveTarget)
  {
  }

  Base::EntityID GetDiveTarget() const
  {
    return _diveTarget;
  }

  bool HasDived() const
  {
    return _hasDived;
  }

  void SetHasDived()
  {
    _hasDived = true;
  }

  void ResetHasDived()
  {
    _hasDived = false;
  }
};
