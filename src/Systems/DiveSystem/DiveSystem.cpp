#include "DiveSystem.hpp"
#include "Components/DiveComponent.hpp"
#include "base/components/TransformComponent.hpp"
#include "raymath.h"

void DiveSystem::Update(float dt, Base::EntityManager *entityManager, const Base::Scene *currentScene)
{
  auto entities_divecmp = entityManager->Query<DiveComponent>();

  for (auto &e : entities_divecmp)
  {
    auto divecmp = e->item->GetComponent<DiveComponent>();
    std::shared_ptr<Base::Entity> target = entityManager->GetEntity(divecmp->GetDiveTarget());
    Vector2 targetPos = target->GetComponent<Base::TransformComponent>()->position;

    auto transcmp = e->item->GetComponent<Base::TransformComponent>();
    Vector2 direction = targetPos - transcmp->position;
  }
}
