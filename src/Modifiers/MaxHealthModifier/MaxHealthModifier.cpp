#include "MaxHealthModifier.hpp"
#include "Components/HealthComponent.hpp"
#include <algorithm>
#include <print>

void MaxHealthModifier::Apply(std::shared_ptr<Base::Entity> entity)
{
  if (entity->HasComponent<HealthComponent>())
  {
    auto hlthcmp = entity->GetComponent<HealthComponent>();
    hlthcmp->maxHealth *= (1 + _maxHealthBoost);
    std::println("Health");
  }
}

void MaxHealthModifier::Remove(std::shared_ptr<Base::Entity> entity)
{
  if (entity->HasComponent<HealthComponent>())
  {
    auto hlthcmp = entity->GetComponent<HealthComponent>();
    hlthcmp->maxHealth /= (1 + _maxHealthBoost);
  }
}

void MaxHealthModifier::SetMaxHealthBoost(float boostPercent)
{
  _maxHealthBoost = boostPercent;
  _maxHealthBoost = std::clamp<float>(_maxHealthBoost, 0, 1);
}

float MaxHealthModifier::GetMaxHealthBoost() const
{
  return _maxHealthBoost;
}
