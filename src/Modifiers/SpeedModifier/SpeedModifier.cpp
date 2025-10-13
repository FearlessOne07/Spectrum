#include "SpeedModifier.hpp"
#include "base/components/MoveComponent.hpp"
#include <algorithm>

void SpeedModifier::Apply(std::shared_ptr<Base::Entity> entity)
{
  if (entity->HasComponent<Base::MoveComponent>())
  {
    auto mvcmp = entity->GetComponent<Base::MoveComponent>();
    mvcmp->driveForce *= (1 + _speedBoost);
    mvcmp->speed *= (1 + _speedBoost);
  }
}

void SpeedModifier::Remove(std::shared_ptr<Base::Entity> entity)
{
  if (entity->HasComponent<Base::MoveComponent>())
  {
    auto mvcmp = entity->GetComponent<Base::MoveComponent>();
    mvcmp->driveForce /= (1 + _speedBoost);
    mvcmp->speed /= (1 + _speedBoost);
  }
}

void SpeedModifier::SetSpeedBoost(float boostPercent)
{
  _speedBoost = boostPercent;
  _speedBoost = std::clamp<float>(_speedBoost, 0, 1);
}

float SpeedModifier::GetSpeedBoost() const
{
  return _speedBoost;
}
