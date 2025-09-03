#pragma once
#include "Modifiers/Modifier.hpp"
class FireRateModifier : public Modifier
{
public:
  void Apply(std::shared_ptr<Base::Entity> entity) override;
  void Remove(std::shared_ptr<Base::Entity> entity) override;
};
