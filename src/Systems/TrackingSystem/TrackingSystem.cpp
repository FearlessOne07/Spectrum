#include "TrackingSystem.hpp"
#include "Components/ShootComponent.hpp"
#include "Components/TrackingComponent.hpp"
#include "Components/TransformEffects.hpp"
#include "raylib/raylib.h"
#include <base/Entity.hpp>
#include <base/EntityManager.hpp>
#include <base/components/RigidBodyComponent.hpp>
#include <base/components/TransformComponent.hpp>
#include <memory>
#include <raymath.h>
#include <vector>

void TrackingSystem::Update(float dt, Base::EntityManager *entityManager)
{
  std::vector<std::shared_ptr<Base::Entity>> entities = entityManager->Query<TrackingComponent>();

  for (auto &e : entities)
  {
    auto *trckcomp = e->GetComponent<TrackingComponent>();
    auto *transcomp = e->GetComponent<Base::TransformComponent>();
    auto *rbcmp = e->GetComponent<Base::RigidBodyComponent>();

    const Base::Entity *target = entityManager->GetEntity(trckcomp->targetEntityID);
    Vector2 targetPos = target->GetComponent<Base::TransformComponent>()->position;

    if (trckcomp->trackingDistance == 0.f ||
        Vector2Distance(targetPos, transcomp->position) > trckcomp->trackingDistance)
    {
      trckcomp->isTracking = true;
      rbcmp->direction = Vector2Subtract( //
        targetPos, transcomp->position    //
      );
    }
    else
    {
      trckcomp->isTracking = false;
      rbcmp->direction = {0, 0};
    }

    if (e->HasComponent<ShootComponent>())
    {
      auto *shtcmp = e->GetComponent<ShootComponent>();
      auto *transfxcmp = e->GetComponent<TransformEffectsComponent>();
      if (!trckcomp->isTracking)
      {
        shtcmp->IsFiring = true;
        shtcmp->target = targetPos;
        transfxcmp->targetAngularVelocity = 270;
      }
      else
      {
        shtcmp->IsFiring = false;
        transfxcmp->targetAngularVelocity = 90;
        shtcmp->bulletFireTimer = 0;
      }
    }
  }
}
