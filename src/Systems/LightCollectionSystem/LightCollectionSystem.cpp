#include "LightCollectionSystem.hpp"
#include "Components/LightCollectorComponent.hpp"
#include "Components/LightComponent.hpp"
#include "base/components/MoveComponent.hpp"
#include "base/components/RigidBodyComponent.hpp"
#include "base/components/TransformComponent.hpp"
#include "base/entities/EntityManager.hpp"
#include "base/util/Circle.hpp"
#include "raymath.h"
#include <algorithm>

void LightCollectionSystem::Update(float dt, Base::EntityManager *entityManager, const Base::Scene *currentScene)
{
  auto entities = entityManager->Query<LightCollectorComponent>();

  for (auto &e : entities)
  {
    auto lightCol = e->item->GetComponent<LightCollectorComponent>();
    auto transcmp = e->item->GetComponent<Base::TransformComponent>();

    auto light = entityManager->QueryArea(Circle{transcmp->position, lightCol->collectionRaius});

    for (auto item : light)
    {
      if (item->item->HasComponent<LightComponent>())
      {
        auto mvcmp = item->item->GetComponent<Base::MoveComponent>();
        auto rbcmp = item->item->GetComponent<Base::RigidBodyComponent>();
        auto lightTranscmp = item->item->GetComponent<Base::TransformComponent>();
        float distSqr = Vector2DistanceSqr(transcmp->position, lightTranscmp->position);
        float colRadiusSqr = lightCol->collectionRaius * lightCol->collectionRaius;

        if (distSqr <= colRadiusSqr)
        {
          float scale = 1 - std::clamp<float>(distSqr / colRadiusSqr, 0, 1);
          mvcmp->driveForce = scale * scale * 4000;
          rbcmp->direction = Vector2Normalize(Vector2Subtract(transcmp->position, lightTranscmp->position));
        }
      }
    }
  }
}
