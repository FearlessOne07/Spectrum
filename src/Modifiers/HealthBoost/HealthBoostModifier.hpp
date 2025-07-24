#pragma once
#include "Modifiers/Modifier.hpp"

class HealthBoostModifier : public Modifier
{
private:
  float _healthBoost = 5;

public:
  void Apply(std::shared_ptr<Base::Entity> entity) override;
  void Remove(std::shared_ptr<Base::Entity> entity) override;
  void SetHealthBoost(float boostPercent);
  float GetHealthBoost() const;
};
