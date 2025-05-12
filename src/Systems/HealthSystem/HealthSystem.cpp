#include "HealthSystem.hpp"
#include "Components/HealthComponent.hpp"
#include "Signals/EntityDamagedSignal.hpp"
#include "Signals/EntityDiedSignal.hpp"
#include "base/signals/SignalManager.hpp"
#include <base/entities/EntityManager.hpp>
#include <memory>
#include <vector>

void HealthSystem::Update(float dt, Base::EntityManager *entityManager)
{
  std::vector<std::shared_ptr<Base::Entity>> entities = entityManager->Query<HealthComponent>();
  auto *bus = Base::SignalManager::GetInstance();

  for (auto &e : entities)
  {
    auto *hlthcmp = e->GetComponent<HealthComponent>();

    if (hlthcmp->hasPendingSickness)
    {
      hlthcmp->health -= hlthcmp->sickness;
      std::shared_ptr<EntityDamagedSignal> sig = std::make_shared<EntityDamagedSignal>();
      sig->entity = e;
      sig->damageTaken = hlthcmp->sickness;
      bus->BroadCastSignal(sig);

      hlthcmp->sickness = 0;
      hlthcmp->hasPendingSickness = false;
    }

    if (hlthcmp->health <= 0 && e->IsAlive())
    {
      e->SetDead();
      std::shared_ptr<EntityDiedSignal> sig = std::make_shared<EntityDiedSignal>();
      sig->entity = e;
      bus->BroadCastSignal(sig);
    }
  }
}
