#pragma once
#include "base/components/Component.hpp"
#include "base/entities/Entity.hpp"
class DiveComponent : public Base::Component
{
private:
  float _diveForce = 0;
  Base::EntityID _diveTarget;
  bool _hasDived = false;

public:
  DiveComponent(float diveForce, Base::EntityID diveTarget) : _diveForce(diveForce), _diveTarget(diveTarget)
  {
  }

  float GetDiveForce() const
  {
    return _diveForce;
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
};
