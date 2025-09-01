#include "DiveSystem.hpp"
#include "Components/DiveComponent.hpp"
#include "base/components/RigidBodyComponent.hpp"
#include "base/components/TransformComponent.hpp"
#include "raymath.h"

void DiveSystem::Update(float dt, Base::EntityManager *entityManager, const Base::Scene *currentScene)
{
  auto entities_dvcmp = entityManager->Query<DiveComponent>();

  for (auto &e : entities_dvcmp)
  {
    auto divecmp = e->item->GetComponent<DiveComponent>();
    auto transcmp = e->item->GetComponent<Base::TransformComponent>();
    auto rbcmp = e->item->GetComponent<Base::RigidBodyComponent>();

    if (!divecmp->HasDived())
    {
      Vector2 targetPos =
        entityManager->GetEntity(divecmp->GetDiveTarget())->GetComponent<Base::TransformComponent>()->position;
      rbcmp->direction = Vector2Normalize(targetPos - transcmp->position);
      divecmp->SetHasDived();
    }
  }
}
