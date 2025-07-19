#pragma once
#include "Modifiers/Modifier.hpp"

class SpeedModifier : public Modifier
{
private:
  float _speedBoost = 2;

public:
  void Apply(std::shared_ptr<Base::Entity> entity) override;
  void Remove(std::shared_ptr<Base::Entity> entity) override;
};
