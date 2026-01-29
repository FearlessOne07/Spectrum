#pragma once
#include "Modifiers/Modifier.hpp"

class AttackSpeedModifier : public Modifier
{
private:
  float _speedBoost = 1;

public:
  void Apply(std::shared_ptr<Base::Entity> entity) override;
  void Remove(std::shared_ptr<Base::Entity> entity) override;
  void SetSpeedBoost(float boostPercent);
  float GetSpeedBoost() const;
};
