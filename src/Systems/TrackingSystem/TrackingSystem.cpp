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
    auto *trckcomp = e->GetComponent<TrackingComponent>();
    auto *transcomp = e->GetComponent<Base::TransformComponent>();
    auto *rbcmp = e->GetComponent<Base::RigidBodyComponent>();
    auto *transfxcmp = e->GetComponent<TransformEffectsComponent>();
    auto *abbcmp = e->GetComponent<Base::ColliderComponent>();

    auto target = entityManager->GetEntity(trckcomp->targetEntityID);
    
    Vector2 targetPos = target->GetComponent<Base::TransformComponent>()->position;

    Vector2 positionMin = {0, 0};
    Vector2 positionMax = {0, 0};

    if (abbcmp->shape == Base::ColliderComponent::Shape::BOX)
    {
      positionMin = {
        transcomp->position.x - abbcmp->positionOffset.x,
        transcomp->position.y - abbcmp->positionOffset.y,
      };

      positionMax = {positionMin.x + abbcmp->size.x, positionMin.y + abbcmp->size.y};
    }
    else if (abbcmp->shape == Base::ColliderComponent::Shape::CIRCLE)
    {
      positionMin = {
        (transcomp->position.x - abbcmp->radius) - abbcmp->positionOffset.x,
        (transcomp->position.y - abbcmp->radius) - abbcmp->positionOffset.y,
      };

      positionMax = {
        (transcomp->position.x + abbcmp->radius) - abbcmp->positionOffset.x,
        (transcomp->position.y + abbcmp->radius) - abbcmp->positionOffset.y,
      };
    }

    bool isInBoundsX = (positionMin.x > transfxcmp->bindMin.x) && (positionMax.x < transfxcmp->bindMax.x);
    bool isInBoundsY = (positionMin.y > transfxcmp->bindMin.y) && (positionMax.y < transfxcmp->bindMax.y);
    bool isInRange = Vector2Distance(targetPos, transcomp->position) < trckcomp->trackingDistance;

    trckcomp->targetDirection = Vector2Subtract( //
      targetPos, transcomp->position             //
    );

    trckcomp->targetDirection = Vector2Normalize(trckcomp->targetDirection);

    if ( //
      trckcomp->trackingDistance == 0.f || !isInRange || !(isInBoundsX && isInBoundsY))
    {
      trckcomp->isTracking = true;
      rbcmp->direction = trckcomp->targetDirection;
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
      auto *abbcmp = e->GetComponent<Base::ColliderComponent>();

      auto rd = Base::RenderContextSingleton::GetInstance();

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
