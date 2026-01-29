#include "AttackSpeedModifier.hpp"
#include "Components/ShootComponent.hpp"
void AttackSpeedModifier::Apply(std::shared_ptr<Base::Entity> entity)
{
  if (entity->HasComponent<ShootComponent>())
  {
    auto shtcmp = entity->GetComponent<ShootComponent>();
    shtcmp->bulletFireRate *= (1 - _speedBoost);
  }
}

void AttackSpeedModifier::Remove(std::shared_ptr<Base::Entity> entity)
{
  if (entity->HasComponent<ShootComponent>())
  {
    auto shtcmp = entity->GetComponent<ShootComponent>();
    shtcmp->bulletFireRate /= (1 - _speedBoost);
  }
}

void AttackSpeedModifier::SetSpeedBoost(float boostPercent)
{
  _speedBoost = boostPercent;
  _speedBoost = std::clamp<float>(_speedBoost, 0, 1);
}

float AttackSpeedModifier::GetSpeedBoost() const
{
  return _speedBoost;
}
