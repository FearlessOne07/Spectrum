#include "TransformEffectsSystem.hpp"
#include "Components/TransformEffects.hpp"
#include "base/components/RigidBodyComponent.hpp"
#include "base/components/TransformComponent.hpp"
#include "base/util/Easings.hpp"
#include "base/util/Math.hpp"
#include <base/components/ColliderComponent.hpp>
#include <base/entities/Entity.hpp>
#include <base/entities/EntityManager.hpp>
#include <cmath>
#include <memory>

void TransformEffectsSystem::Update(                                                                //
  float dt, Base::Ref<Base::EntityManager> entityManager, std::shared_ptr<Base::Scene> currentScene //
)
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
        Base::Vector2 targetPos = target->GetComponent<Base::TransformComponent>()->position;
        float distance = Base::Math::Length(transcmp->position - targetPos);
        if (distance <= transfxcmp->lookAtDistance || transfxcmp->lookAtDistance == 0)
        {
          float angle = atan2f(targetPos.y - transcmp->position.y, targetPos.x - transcmp->position.x);
          float targetRotation = Base::Math::ToDegrees(angle) + 90;
          float delta = fmodf(targetRotation - transcmp->rotation + 540.0f, 360.0f) - 180.0f;
          transcmp->rotation += delta * Base::Easings::EaseOutCubic(dt);
          transcmp->rotation = fmodf(transcmp->rotation, 360.f);
        }
      }
      else
      {
        transfxcmp->angularVelocity = Base::Math::Lerp(                                                        //
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

        Base::Vector2 positionMin = {0, 0};
        Base::Vector2 positionMax = {0, 0};

        if (abbcmp->shape == Base::ColliderComponent::Shape::Box)
        {
          positionMin = {
            transcmp->position.x - abbcmp->positionOffset.x,
            transcmp->position.y - abbcmp->positionOffset.y,
          };

          positionMax = {positionMin.x + abbcmp->size.x, positionMin.y + abbcmp->size.y};
        }
        else if (abbcmp->shape == Base::ColliderComponent::Shape::Circle)
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

          if (abbcmp->shape == Base::ColliderComponent::Shape::Box)
          {
            transcmp->position.x = transfxcmp->bindMin.x + abbcmp->positionOffset.x;
          }
          else if (abbcmp->shape == Base::ColliderComponent::Shape::Circle)
          {
            transcmp->position.x = (transfxcmp->bindMin.x + abbcmp->radius) + abbcmp->positionOffset.x;
          }

          rbcmp->velocity.x = 0;
        }
        else if (positionMax.x > transfxcmp->bindMax.x)
        {

          if (abbcmp->shape == Base::ColliderComponent::Shape::Box)
          {
            transcmp->position.x = transfxcmp->bindMax.x - abbcmp->positionOffset.x;
          }
          else if (abbcmp->shape == Base::ColliderComponent::Shape::Circle)
          {
            transcmp->position.x = (transfxcmp->bindMax.x - abbcmp->radius) - abbcmp->positionOffset.x;
          }
          rbcmp->velocity.x = 0;
        }

        if (positionMin.y < transfxcmp->bindMin.y)
        {
          if (abbcmp->shape == Base::ColliderComponent::Shape::Box)
          {
            transcmp->position.y = transfxcmp->bindMin.y + abbcmp->positionOffset.y;
          }
          else if (abbcmp->shape == Base::ColliderComponent::Shape::Circle)
          {
            transcmp->position.y = (transfxcmp->bindMin.y + abbcmp->radius) + abbcmp->positionOffset.y;
          }
          rbcmp->velocity.y = 0;
        }
        else if (positionMax.y > transfxcmp->bindMax.y)
        {
          if (abbcmp->shape == Base::ColliderComponent::Shape::Box)
          {
            transcmp->position.y = transfxcmp->bindMax.y - abbcmp->positionOffset.y;
          }
          else if (abbcmp->shape == Base::ColliderComponent::Shape::Circle)
          {
            transcmp->position.y = (transfxcmp->bindMax.y - abbcmp->radius) - abbcmp->positionOffset.y;
          }
          rbcmp->velocity.y = 0;
        }
      }
    }
  }
}
