#pragma once
#include <base/entities/Entity.hpp>

class Modifier
{
public:
  virtual ~Modifier()
  {
  }
  virtual void Apply(std::shared_ptr<Base::Entity>) = 0;
  virtual void Remove(std::shared_ptr<Base::Entity>) = 0;
  virtual void Tick(float dt) {};
};
