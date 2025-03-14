#pragma once
#include "Gems/Gem.hpp"

class RedGem : public Gem
{
private:
  float _speedIncreaseFactor = 1.2;

public:
  RedGem() = default;
  void Equip(Base::Entity *entity) override;
  void Unequip(Base::Entity *entity) override;
};
