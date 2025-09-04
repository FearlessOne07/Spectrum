#pragma once
#include <base/components/Component.hpp>

class DamageComponent : public Base::Component
{
private:
  float _damage = 0;
  float _baseDamage = 0;

public:
  DamageComponent(float baseDamage) : _baseDamage(baseDamage), _damage(baseDamage)
  {
  }

  float GetBaseDamage() const
  {
    return _baseDamage;
  }

  float GetDamage() const
  {
    return _damage;
  }

  void SetDamage(float damage)
  {
    _damage = damage;
  }
};
