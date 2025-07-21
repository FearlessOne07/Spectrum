#pragma once
#include "Modifiers/Modifier.hpp"

class MaxHealthModifier : public Modifier
{
private:
  float _maxHealthBoost = 0.5;

public:
  void Apply(std::shared_ptr<Base::Entity> entity) override;
  void Remove(std::shared_ptr<Base::Entity> entity) override;
  void SetMaxHealthBoost(float boostPercent);
  float GetMaxHealthBoost() const;
};
