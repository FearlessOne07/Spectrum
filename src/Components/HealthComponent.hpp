#pragma once
#include <base/components/Component.hpp>

struct HealthComponent : public Base::Component
{
  float health = 0;
  float sickness = 0;
  bool hasPendingSickness = false;
};
