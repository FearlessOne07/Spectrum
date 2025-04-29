#include "TransformEffectsSystem.hpp"
#include "Components/TransformEffects.hpp"
#include "base/components/RigidBodyComponent.hpp"
#include "base/components/TransformComponent.hpp"
#include <base/components/ColliderComponent.hpp>
#include <base/entities/Entity.hpp>
#include <base/entities/EntityManager.hpp>
#include <memory>
#include <raylib.h>
#include <raymath.h>
#include <vector>

void TransformEffectsSystem::Update(float dt, Base::EntityManager *entityManager)
{
  std::vector<std::shared_ptr<Base::Entity>> entities = entityManager->Query<TransformEffectsComponent>();

  for (auto &e : entities)
  {
    auto *transfxcmp = e->GetComponent<TransformEffectsComponent>();
    auto *transcmp = e->GetComponent<Base::TransformComponent>();

    if (transfxcmp->rotate)
    {
      transfxcmp->angularVelocity = Lerp(                                                                    //
        transfxcmp->angularVelocity, transfxcmp->targetAngularVelocity, transfxcmp->angularAcceleration * dt //
      );
      transcmp->rotation += transfxcmp->angularVelocity * dt;
    }

    if (transfxcmp->bind)
    {
      if (e->HasComponent<Base::ColliderComponent>())
      {

        auto *rbcmp = e->GetComponent<Base::RigidBodyComponent>();
        auto *abbcmp = e->GetComponent<Base::ColliderComponent>();

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
