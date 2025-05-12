#pragma once

#include "base/entities/Entity.hpp"
#include "base/signals/Signal.hpp"
#include <memory>

class PlayerSpawnedSignal : public Base::Signal
{
public:
  const Base::Entity *player = nullptr;
};
