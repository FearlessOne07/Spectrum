
#pragma once
#include <base/components/Component.hpp>

enum class EnemyType : uint8_t
{
  NONE = 0,
  CHASER,
  SHOOTER,
  KAMAKAZI
};

struct EnemyComponent : public Base::Component
{
  int value = 0;
  EnemyType type = EnemyType::NONE;
};
