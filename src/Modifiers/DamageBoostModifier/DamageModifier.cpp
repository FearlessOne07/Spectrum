
#include "DamageModifier.hpp"
#include "Components/DamageComponent.hpp"
void DamageModifier::Apply(std::shared_ptr<Base::Entity> entity)
{
  if (entity->HasComponent<DamageComponent>())
  {
    auto dmgcmp = entity->GetComponent<DamageComponent>();
    dmgcmp->SetDamage(dmgcmp->GetDamage() * (1 + _damageBoost));
  }
}

void DamageModifier::Remove(std::shared_ptr<Base::Entity> entity)
{
  if (entity->HasComponent<DamageComponent>())
  {
    auto dmgcmp = entity->GetComponent<DamageComponent>();
    dmgcmp->SetDamage(dmgcmp->GetDamage() / (1 + _damageBoost));
  }
}

void DamageModifier::SetDamageBoost(float boostPercent)
{
  _damageBoost = boostPercent;
}

float DamageModifier::GetDamageBoost() const
{
  return _damageBoost;
}
