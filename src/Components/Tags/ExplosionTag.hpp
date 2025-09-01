#pragma once
#include <base/components/Component.hpp>

class ExplosionTag : public Base::Component
{
public:
  float explosionForce = 0;
  bool exploded = false;

  ExplosionTag(float _explosionForce) : explosionForce(_explosionForce)
  {
  }
};
