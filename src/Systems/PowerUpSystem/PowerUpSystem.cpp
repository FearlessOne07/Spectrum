#include "PowerUpSystem.hpp"
#include "Components/PowerUpComponent.hpp"
#include "base/entities/EntityManager.hpp"

void PowerUpSystem::Update(float dt, Base::EntityManager *entityManager, const Base::Scene *)
{
  auto entities_pucmp = entityManager->Query<PowerUpComponent>();

  for (auto &item : entities_pucmp)
  {
    auto e = item->item;
    auto pucmp = e->GetComponent<PowerUpComponent>();

    for (auto it = pucmp->GetPowerUps().begin(); it != pucmp->GetPowerUps().end();)
    {
      if (!it->IsApplied())
      {
        it->Apply(e);
        it++;
      }
      else
      {
        if (it->GetTimeLeft() <= 0)
        {
          it->Remove(e);
          it = pucmp->RemovePowerUp(it);
        }
        else
        {
          it->DecrmentDuration(dt);
          it++;
        }
      }
    }
  }
}
