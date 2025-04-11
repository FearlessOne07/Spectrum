#include "TrackingSystem.hpp"
#include "Components/TrackingComponent.hpp"
#include "base/Entity.hpp"
#include "base/EntityManager.hpp"
#include "base/components/RigidBodyComponent.hpp"
#include "base/components/TransformComponent.hpp"
#include "raymath.h"
#include <memory>
#include <vector>

void TrackingSystem::Update(float dt, Base::EntityManager *entityManager)
{
  std::vector<std::shared_ptr<Base::Entity>> entities = entityManager->Query<TrackingComponent>();

  for (auto &e : entities)
  {
    auto *trckcomp = e->GetComponent<TrackingComponent>();
    auto *transcomp = e->GetComponent<Base::TransformComponent>();
    auto *rbcmp = e->GetComponent<Base::RigidBodyComponent>();

    const Base::Entity *e2 = entityManager->GetEntity(trckcomp->targetEntityID);
    rbcmp->direction = Vector2Subtract(                                           //
      e2->GetComponent<Base::TransformComponent>()->position, transcomp->position //
    );
  }
}
