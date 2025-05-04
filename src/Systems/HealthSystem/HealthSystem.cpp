#include "HealthSystem.hpp"
#include "Components/HealthComponent.hpp"
#include "Signals/EntityDiedSignal.hpp"
#include "base/signals/SignalManager.hpp"
#include <base/entities/EntityManager.hpp>
#include <memory>
#include <vector>

void HealthSystem::Update(float dt, Base::EntityManager *entityManager)
{
  std::vector<std::shared_ptr<Base::Entity>> entities = entityManager->Query<HealthComponent>();

  for (auto &e : entities)
  {
    auto *hlthcmp = e->GetComponent<HealthComponent>();
    if (hlthcmp->health <= 0 && e->IsAlive())
    {
      e->SetDead();
      auto *bus = Base::SignalManager::GetInstance();
      std::shared_ptr<EntityDiedSignal> sig = std::make_shared<EntityDiedSignal>();
      sig->entity = e;
      bus->BroadCastSignal(sig);
    }
  }
}
