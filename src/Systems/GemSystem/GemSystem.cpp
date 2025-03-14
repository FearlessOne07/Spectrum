#include "GemSystem.hpp"
#include "Components/GemComponent/GemComponent.hpp"
#include "Gems/RedGem/RedGem.hpp"
#include "base/EntityManager.hpp"
#include <memory>

void GemSystem::Update(float dt, Base::EntityManager *entityManager)
{
  std::vector<std::shared_ptr<Base::Entity>> entities = entityManager->Query<GemComponent>();

  for (auto &e : entities)
  {
    auto *gemcmp = e->GetComponent<GemComponent>();
    if (gemcmp->eqiupRequest != std::type_index(typeid(nullptr)))
    {
      if (gemcmp->currentGem != nullptr)
      {
        gemcmp->currentGem->Unequip(e.get());
      }
      gemcmp->currentGem = std::make_unique<RedGem>();
      gemcmp->currentGem->Equip(e.get());
      gemcmp->eqiupRequest = std::type_index(typeid(nullptr));
    }
  }
}
