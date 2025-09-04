#pragma once
#include "Modifiers/Modifier.hpp"

class DamageModifier : public Modifier
{
private:
  float _damageBoost = 1;

public:
  void Apply(std::shared_ptr<Base::Entity> entity) override;
  void Remove(std::shared_ptr<Base::Entity> entity) override;
  void SetDamageBoost(float boostPercent);
  float GetDamageBoost() const;
};
