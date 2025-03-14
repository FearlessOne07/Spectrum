#pragma once

#include "base/Entity.hpp"
class Gem
{

public:
  virtual ~Gem() = default;
  virtual void Equip(Base::Entity *entity) = 0;
  virtual void Unequip(Base::Entity *entity) = 0;
};
