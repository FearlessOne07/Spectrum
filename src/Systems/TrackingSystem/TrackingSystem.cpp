#include "TrackingSystem.hpp"
#include "Components/TrackingComponent.hpp"
#include <base/components/ColliderComponent.hpp>
#include <base/components/RigidBodyComponent.hpp>
#include <base/components/TransformComponent.hpp>
#include <base/entities/Entity.hpp>
#include <base/entities/EntityManager.hpp>
#include <base/renderer/RenderContextSingleton.hpp>
#include <memory>

void TrackingSystem::Update(                                                                              //
  float dt, Base::Ref<Base::EntityManager> entityManager, std::shared_ptr<const Base::Scene> currentScene //
)
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
