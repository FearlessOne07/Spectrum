#include "MaxHealthModifier.hpp"
#include "Components/HealthComponent.hpp"
#include <algorithm>

void MaxHealthModifier::Apply(std::shared_ptr<Base::Entity> entity)
{
  if (entity->HasComponent<HealthComponent>())
  {
    auto hlthcmp = entity->GetComponent<HealthComponent>();
    hlthcmp->SetMaxHealth(hlthcmp->GetMaxHealth() * (1 + _maxHealthBoost));
    hlthcmp->SetHealth(hlthcmp->GetHealth() * (1 + _maxHealthBoost));
  }
}

void MaxHealthModifier::Remove(std::shared_ptr<Base::Entity> entity)
{
  if (entity->HasComponent<HealthComponent>())
  {
    auto hlthcmp = entity->GetComponent<HealthComponent>();
    hlthcmp->SetMaxHealth(hlthcmp->GetMaxHealth() / (1 + _maxHealthBoost));
    hlthcmp->SetHealth(hlthcmp->GetHealth() / (1 + _maxHealthBoost));
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
