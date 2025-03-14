#include "RedGem.hpp"
#include "base/components/MoveComponent.hpp"
#include "base/components/ShapeComponent.hpp"

void RedGem::Equip(Base::Entity *entity)
{
  auto *mvcmp = entity->GetComponent<Base::MoveComponent>();
  mvcmp->speed *= 1.2;

  auto *shpcmp = entity->GetComponent<Base::ShapeComponent>();
  shpcmp->color = BLUE;
}

void RedGem::Unequip(Base::Entity *entity)
{
  auto *mvcmp = entity->GetComponent<Base::MoveComponent>();
  mvcmp->speed /= 1.2;
}
