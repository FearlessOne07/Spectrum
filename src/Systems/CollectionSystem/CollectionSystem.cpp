#include "CollectionSystem.hpp"
#include "Components/CollectableComponent.hpp"
#include "Components/Tags/Collector.hpp"
#include "base/components/MoveComponent.hpp"
#include "base/components/RigidBodyComponent.hpp"
#include "base/components/TransformComponent.hpp"
#include "base/entities/EntityManager.hpp"
#include "base/util/Circle.hpp"
#include <algorithm>

void CollectionSystem::Update(                                                                            ///
  float dt, Base::Ref<Base::EntityManager> entityManager, std::shared_ptr<const Base::Scene> currentScene //
)
{
  auto entities = entityManager->Query<Collector>();

  for (auto &e : entities)
  {
    auto colcmp = e->item->GetComponent<Collector>();
    auto transcmp = e->item->GetComponent<Base::TransformComponent>();

    auto light = entityManager->QueryArea(Circle{transcmp->position, colcmp->collectionRadius});

    for (auto item : light)
    {
      if (item->item->HasComponent<CollectableComponent>())
      {
        auto mvcmp = item->item->GetComponent<Base::MoveComponent>();
        auto rbcmp = item->item->GetComponent<Base::RigidBodyComponent>();
        auto lightTranscmp = item->item->GetComponent<Base::TransformComponent>();
        float distSqr = Vector2DistanceSqr(transcmp->position, lightTranscmp->position);
        float colRadiusSqr = colcmp->collectionRadius * colcmp->collectionRadius;

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
