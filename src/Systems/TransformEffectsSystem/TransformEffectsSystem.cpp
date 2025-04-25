#include "TransformEffectsSystem.hpp"
#include "Components/TransformEffects.hpp"
#include "base/components/MoveComponent.hpp"
#include "base/components/RigidBodyComponent.hpp"
#include "base/components/TransformComponent.hpp"
#include "raylib/raymath.h"
#include <base/Entity.hpp>
#include <base/EntityManager.hpp>
#include <base/components/ColliderComponent.hpp>
#include <memory>
#include <raylib.h>
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
        Vector2 positionMin = {
          transcmp->position.x - abbcmp->positionOffset.x,
          transcmp->position.y - abbcmp->positionOffset.y,
        };
        Vector2 positionMax = {positionMin.x + abbcmp->size.x, positionMin.y + abbcmp->size.y};

        if (positionMin.x < transfxcmp->bindMin.x)
        {
          transcmp->position.x = transfxcmp->bindMin.x + abbcmp->positionOffset.x;
          rbcmp->velocity.x = 0;
        }
        else if (positionMax.x > transfxcmp->bindMax.x)
        {
          transcmp->position.x = transfxcmp->bindMax.x - abbcmp->positionOffset.x;
          rbcmp->velocity.x = 0;
        }

        if (positionMin.y < transfxcmp->bindMin.y)
        {
          transcmp->position.y = transfxcmp->bindMin.y + abbcmp->positionOffset.y;
          rbcmp->velocity.y = 0;
        }
        else if (positionMax.y > transfxcmp->bindMax.y)
        {
          transcmp->position.y = transfxcmp->bindMax.y - abbcmp->positionOffset.y;
          rbcmp->velocity.y = 0;
        }
      }
    }
  }
}
