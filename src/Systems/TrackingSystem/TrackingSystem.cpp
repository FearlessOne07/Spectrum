#include "TrackingSystem.hpp"
#include "Components/ShootComponent.hpp"
#include "Components/TrackingComponent.hpp"
#include "Components/TransformEffects.hpp"
#include <base/components/ColliderComponent.hpp>
#include <base/components/RigidBodyComponent.hpp>
#include <base/components/TransformComponent.hpp>
#include <base/entities/Entity.hpp>
#include <base/entities/EntityManager.hpp>
#include <base/renderer/RenderContext.hpp>
#include <base/renderer/RenderContextSingleton.hpp>
#include <memory>
#include <raylib.h>
#include <raymath.h>

void TrackingSystem::Update(float dt, Base::EntityManager *entityManager, const Base::Scene *currentScene)
{
  auto entities = entityManager->Query<TrackingComponent>();

  for (auto &item : entities)
  {
    auto e = item->item;
    auto trckcomp = e->GetComponent<TrackingComponent>();
    auto transcomp = e->GetComponent<Base::TransformComponent>();
    auto rbcmp = e->GetComponent<Base::RigidBodyComponent>();

    Vector2 targetPos = {0, 0};
    if (trckcomp->GetTargetEntityID())
    {
      auto target = entityManager->GetEntity(trckcomp->GetTargetEntityID());
      targetPos = target->GetComponent<Base::TransformComponent>()->position;
    }
    else
    {
      targetPos = trckcomp->GetTargetPosition();
    }

    Vector2 trackingDirection = Vector2Subtract( //
      targetPos, transcomp->position             //
    );
    rbcmp->direction = Vector2Normalize(trackingDirection);
  }
}
