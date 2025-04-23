#include "HealthSystem.hpp"
#include "Components/HealthComponent.hpp"
#include <base/EntityManager.hpp>
#include <memory>
#include <vector>

void HealthSystem::Update(float dt, Base::EntityManager *entityManager)
{
  std::vector<std::shared_ptr<Base::Entity>> entities = entityManager->Query<HealthComponent>();

  for (auto &e : entities)
  {
    auto *hlthcmp = e->GetComponent<HealthComponent>();
    if (hlthcmp->health <= 0)
    {
      e->SetDead();
    }
  }
}
