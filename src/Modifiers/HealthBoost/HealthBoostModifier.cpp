#include "HealthBoostModifier.hpp"
#include "Components/HealthComponent.hpp"

void HealthBoostModifier::Apply(std::shared_ptr<Base::Entity> entity)
{
  if (entity->HasComponent<HealthComponent>())
  {
    auto hlthcmp = entity->GetComponent<HealthComponent>();
    hlthcmp->SetHealth(hlthcmp->GetHealth() + _healthBoost);
  }
}

void HealthBoostModifier::Remove(std::shared_ptr<Base::Entity> entity)
{
  if (entity->HasComponent<HealthComponent>())
  {
    auto hlthcmp = entity->GetComponent<HealthComponent>();
    hlthcmp->SetHealth(hlthcmp->GetHealth() + _healthBoost);
  }
}

void HealthBoostModifier::SetHealthBoost(float boost)
{
  _healthBoost = boost;
}

float HealthBoostModifier::GetHealthBoost() const
{
  return _healthBoost;
}
