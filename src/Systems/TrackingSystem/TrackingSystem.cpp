#include "TrackingSystem.hpp"
#include "Components/TrackingComponent/TrackingComponent.hpp"
#include "base/Entity.hpp"
#include "base/EntityManager.hpp"
#include "base/components/MoveComponent.hpp"
#include "base/components/TransformComponent.hpp"
#include "raymath.h"
#include <memory>
#include <vector>

void TrackingSystem::Update(float dt, Base::EntityManager *entityManager)
{
  std::vector<std::shared_ptr<Base::Entity>> entities = entityManager->Query<TrackingComponent>();

  for (auto &e : entities)
  {
    auto *mvcomp = e->GetComponent<Base::MoveComponent>();
    auto *trckcomp = e->GetComponent<TrackingComponent>();
    auto *transcomp = e->GetComponent<Base::TransformComponent>();

    const Base::Entity *e2 = entityManager->GetEntity(trckcomp->targetEntityID);
    mvcomp->targetVelocity = Vector2Subtract(                                     //
      e2->GetComponent<Base::TransformComponent>()->position, transcomp->position //
    );
  }
}

void TrackingSystem::Stop()
{
}

void TrackingSystem::Start()
{
}
