#include "SpeedModifier.hpp"
#include "base/components/MoveComponent.hpp"
#include "base/components/RigidBodyComponent.hpp"

void SpeedModifier::Apply(std::shared_ptr<Base::Entity> entity)
{
  if (entity->HasComponent<Base::MoveComponent>())
  {
    auto mvcmp = entity->GetComponent<Base::MoveComponent>();
    mvcmp->driveForce *= _speedBoost;
  }
  else if (entity->HasComponent<Base::RigidBodyComponent>())
  {
    auto rbcmp = entity->GetComponent<Base::RigidBodyComponent>();
    rbcmp->speed *= _speedBoost;
  }
}

void SpeedModifier::Remove(std::shared_ptr<Base::Entity> entity)
{
  if (entity->HasComponent<Base::MoveComponent>())
  {
    auto mvcmp = entity->GetComponent<Base::MoveComponent>();
    mvcmp->driveForce /= _speedBoost;
  }
  else if (entity->HasComponent<Base::RigidBodyComponent>())
  {
    auto rbcmp = entity->GetComponent<Base::RigidBodyComponent>();
    rbcmp->speed /= _speedBoost;
  }
}
