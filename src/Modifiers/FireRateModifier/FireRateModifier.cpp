#include "FireRateModifier.hpp"
#include "Components/ShootComponent.hpp"

void FireRateModifier::Apply(std::shared_ptr<Base::Entity> entity)
{
  if (entity->HasComponent<ShootComponent>())
  {
    auto shtcmp = entity->GetComponent<ShootComponent>();
  }
}

void FireRateModifier::Remove(std::shared_ptr<Base::Entity> entity)
{
  auto shtcmp = entity->GetComponent<ShootComponent>();
}
