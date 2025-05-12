#pragma once
#include "base/entities/Entity.hpp"
#include "base/signals/Signal.hpp"
#include <memory>

class EntityDamagedSignal : public Base::Signal
{
public:
  std::shared_ptr<Base::Entity> entity = nullptr;
  float damageTaken = 0;
};
