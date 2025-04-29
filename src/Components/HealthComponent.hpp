#pragma once
#include <base/components/Component.hpp>

struct HealthComponent : public Base::Component
{
  float health = 0;
};
