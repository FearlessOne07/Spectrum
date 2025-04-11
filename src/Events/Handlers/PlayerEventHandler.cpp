#include "PlayerEventHandler.hpp"
#include "Components/Tags/EnemyTag.hpp"
#include "Components/Tags/PlayerTag.hpp"
#include "base/Event.hpp"
#include "base/EventBus.hpp"
#include "base/components/ImpulseComponent.hpp"
#include "base/components/MoveComponent.hpp"
#include "base/components/RigidBodyComponent.hpp"
#include "base/events/EntityCollisionEvent.hpp"
#include <iostream>
#include <memory>

void PlayerEventHandler::Init()
{
  auto eventBus = Base::EventBus::GetInstance();

  eventBus->SubscribeEvent<Base::EntityCollisionEvent>(
    [this](const std::shared_ptr<Base::Event> &event) { this->PlayerEnemyCollisionHandler(event); } //
  );
}

void PlayerEventHandler::PlayerEnemyCollisionHandler(const std::shared_ptr<Base::Event> event)
{
  auto collEvent = std::static_pointer_cast<Base::EntityCollisionEvent>(event);

  auto attack = collEvent->hittBoxEntity;
  auto defence = collEvent->hurtBoxEntity;

  if (attack->HasComponent<EnemyTag>() && defence->HasComponent<PlayerTag>())
  {
    std::cout << "Col\n";
    auto *impcmp = defence->GetComponent<Base::ImpulseComponent>();
    impcmp->force = attack->GetComponent<Base::MoveComponent>()->driveForce * 1.1;
    impcmp->direction = attack->GetComponent<Base::RigidBodyComponent>()->direction;
  }
}
