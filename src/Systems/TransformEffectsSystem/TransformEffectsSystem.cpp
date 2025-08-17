#include "TransformEffectsSystem.hpp"
#include "Components/TransformEffects.hpp"
#include "base/components/RigidBodyComponent.hpp"
#include "base/components/TransformComponent.hpp"
#include "base/util/Easings.hpp"
#include <base/components/ColliderComponent.hpp>
#include <base/entities/Entity.hpp>
#include <base/entities/EntityManager.hpp>
#include <cmath>
#include <memory>
#include <raylib.h>
#include <raymath.h>

void TransformEffectsSystem::Update(float dt, Base::EntityManager *entityManager, const Base::Scene *currentScene)
{
  auto entities = entityManager->Query<TransformEffectsComponent>();

  for (auto &item : entities)
  {
    auto e = item->item;
    auto transfxcmp = e->GetComponent<TransformEffectsComponent>();
    auto transcmp = e->GetComponent<Base::TransformComponent>();

    if (transfxcmp->rotate)
    {
      auto target = entityManager->GetEntity(transfxcmp->lookAtTarget);
      if (transfxcmp->lookAt && target)
      {
        Vector2 targetPos = target->GetComponent<Base::TransformComponent>()->position;
        float distance = Vector2Distance(transcmp->position, targetPos);
        if (distance <= transfxcmp->lookAtDistance || transfxcmp->lookAtDistance == 0)
        {
          float angle = atan2f(targetPos.y - transcmp->position.y, targetPos.x - transcmp->position.x);
          float targetRotation = (angle * RAD2DEG) + 90;
          float delta = fmodf(targetRotation - transcmp->rotation + 540.0f, 360.0f) - 180.0f;
          transcmp->rotation += delta * Base::Easings::EaseOutCubic(dt);
        }
      }
      else
      {
        transfxcmp->angularVelocity = Lerp(                                                                    //
          transfxcmp->angularVelocity, transfxcmp->targetAngularVelocity, transfxcmp->angularAcceleration * dt //
        );
        transcmp->rotation += transfxcmp->angularVelocity * dt;
      }
    }

    if (transfxcmp->bind)
    {
      if (e->HasComponent<Base::ColliderComponent>())
      {
        auto rbcmp = e->GetComponent<Base::RigidBodyComponent>();
        auto abbcmp = e->GetComponent<Base::ColliderComponent>();

        Vector2 positionMin = {0, 0};
        Vector2 positionMax = {0, 0};

        if (abbcmp->shape == Base::ColliderComponent::Shape::BOX)
        {
          positionMin = {
            transcmp->position.x - abbcmp->positionOffset.x,
            transcmp->position.y - abbcmp->positionOffset.y,
          };

          positionMax = {positionMin.x + abbcmp->size.x, positionMin.y + abbcmp->size.y};
        }
        else if (abbcmp->shape == Base::ColliderComponent::Shape::CIRCLE)
        {
          positionMin = {
            (transcmp->position.x - abbcmp->radius) - abbcmp->positionOffset.x,
            (transcmp->position.y - abbcmp->radius) - abbcmp->positionOffset.y,
          };

          positionMax = {
            (transcmp->position.x + abbcmp->radius) - abbcmp->positionOffset.x,
            (transcmp->position.y + abbcmp->radius) - abbcmp->positionOffset.y,
          };
        }

        if (positionMin.x < transfxcmp->bindMin.x)
        {

          if (abbcmp->shape == Base::ColliderComponent::Shape::BOX)
          {
            transcmp->position.x = transfxcmp->bindMin.x + abbcmp->positionOffset.x;
          }
          else if (abbcmp->shape == Base::ColliderComponent::Shape::CIRCLE)
          {
            transcmp->position.x = (transfxcmp->bindMin.x + abbcmp->radius) + abbcmp->positionOffset.x;
          }

          rbcmp->velocity.x = 0;
        }
        else if (positionMax.x > transfxcmp->bindMax.x)
        {

          if (abbcmp->shape == Base::ColliderComponent::Shape::BOX)
          {
            transcmp->position.x = transfxcmp->bindMax.x - abbcmp->positionOffset.x;
          }
          else if (abbcmp->shape == Base::ColliderComponent::Shape::CIRCLE)
          {
            transcmp->position.x = (transfxcmp->bindMax.x - abbcmp->radius) - abbcmp->positionOffset.x;
          }
          rbcmp->velocity.x = 0;
        }

        if (positionMin.y < transfxcmp->bindMin.y)
        {
          if (abbcmp->shape == Base::ColliderComponent::Shape::BOX)
          {
            transcmp->position.y = transfxcmp->bindMin.y + abbcmp->positionOffset.y;
          }
          else if (abbcmp->shape == Base::ColliderComponent::Shape::CIRCLE)
          {
            transcmp->position.y = (transfxcmp->bindMin.y + abbcmp->radius) + abbcmp->positionOffset.y;
          }
          rbcmp->velocity.y = 0;
        }
        else if (positionMax.y > transfxcmp->bindMax.y)
        {
          if (abbcmp->shape == Base::ColliderComponent::Shape::BOX)
          {
            transcmp->position.y = transfxcmp->bindMax.y - abbcmp->positionOffset.y;
          }
          else if (abbcmp->shape == Base::ColliderComponent::Shape::CIRCLE)
          {
            transcmp->position.y = (transfxcmp->bindMax.y - abbcmp->radius) - abbcmp->positionOffset.y;
          }
          rbcmp->velocity.y = 0;
        }
      }
    }
  }
}
