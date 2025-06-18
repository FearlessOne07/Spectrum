#pragma once
#include "Modifiers/Modifier.hpp"
#include "base/components/MoveComponent.hpp"
#include "base/components/RigidBodyComponent.hpp"

class SpeedModifier : public Modifier
{
private:
  float _speedBoost = 2;

public:
  void Apply(std::shared_ptr<Base::Entity> entity) override
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

  void Remove(std::shared_ptr<Base::Entity> entity) override
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
};
