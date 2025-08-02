#include "HealthSystem.hpp"
#include "Components/HealthComponent.hpp"
#include "Signals/EntityDamagedSignal.hpp"
#include "Signals/EntityDiedSignal.hpp"
#include "base/signals/SignalBus.hpp"
#include <base/entities/EntityManager.hpp>
#include <memory>

void HealthSystem::Update(float dt, Base::EntityManager *entityManager, const Base::Scene *currentScene)
{
  auto entities = entityManager->Query<HealthComponent>();
  auto *bus = Base::SignalBus::GetInstance();

  for (auto &item : entities)
  {
    auto e = item->item;
    auto *hlthcmp = e->GetComponent<HealthComponent>();

    if (hlthcmp->TookDamage())
    {
      std::shared_ptr<EntityDamagedSignal> sig = std::make_shared<EntityDamagedSignal>();
      sig->entity = e;
      sig->damageTaken = hlthcmp->GetLastTakenDamage();
      bus->BroadCastSignal(sig);
    }
    hlthcmp->Reset();
    if (hlthcmp->GetHealth() <= 0 && e->IsAlive())
    {
      e->SetDead();
      std::shared_ptr<EntityDiedSignal> sig = std::make_shared<EntityDiedSignal>();
      sig->entity = e;
      bus->BroadCastSignal(sig);
    }
  }
}
