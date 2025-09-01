#pragma once
#include "base/components/Component.hpp"
class KnockBackComponent : public Base::Component
{
private:
  float _knockBackForce = 0;

public:
  KnockBackComponent(float knockBackForce) : _knockBackForce(knockBackForce)
  {
  }

  float GetForce() const
  {
    return _knockBackForce;
  }
};
