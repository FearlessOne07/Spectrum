#include "PlayerEventHandler.hpp"
#include "Components/BulletComponent.hpp"
#include "Components/DamageComponent.hpp"
#include "Components/ShootComponent.hpp"
#include "Components/Tags/EnemyTag.hpp"
#include "Components/Tags/PlayerTag.hpp"
#include <base/components/ImpulseComponent.hpp>
#include <base/components/MoveComponent.hpp>
#include <base/components/RigidBodyComponent.hpp>
#include <base/events/EntityCollisionEvent.hpp>
#include <base/signals/Event.hpp>
#include <base/signals/EventBus.hpp>
#include <raylib.h>
#include <raymath.h>

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

  if (defence->HasComponent<PlayerTag>())
  {
    if (                                                                        //
      attack->HasComponent<BulletComponent>() &&                                //
      attack->GetComponent<BulletComponent>()->sender->HasComponent<EnemyTag>() //
    )
    {
      // Bullet
      auto dmgcmp = attack->GetComponent<DamageComponent>();
      auto bulcmp = attack->GetComponent<BulletComponent>();
      auto rbcmp = attack->GetComponent<Base::RigidBodyComponent>();
      attack->SetDead();

      auto *impcmp = defence->GetComponent<Base::ImpulseComponent>();
      impcmp->force = bulcmp->sender->GetComponent<ShootComponent>()->bulletKnockbackForce;
      impcmp->direction = rbcmp->direction;
    }
    else if (attack->HasComponent<EnemyTag>())
    {

      Vector2 attackDir = attack->GetComponent<Base::RigidBodyComponent>()->direction;
      auto *impcmp = defence->GetComponent<Base::ImpulseComponent>();

      if (Vector2Length(attackDir) == 0)
      {
        auto *defrbcmp = defence->GetComponent<Base::RigidBodyComponent>();
        impcmp->direction = defrbcmp->direction;
      }
      else
      {
        impcmp->direction = attackDir;
      }

      impcmp->force = attack->GetComponent<Base::MoveComponent>()->driveForce * 2;
    }
  }
}
