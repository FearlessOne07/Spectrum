#include "PlayerEventHandler.hpp"
#include "Components/BulletComponent.hpp"
#include "Components/DamageComponent.hpp"
#include "Components/ShootComponent.hpp"
#include "Components/Tags/EnemyTag.hpp"
#include "Components/Tags/PlayerTag.hpp"
#include <base/components/ImpulseComponent.hpp>
#include <base/components/MoveComponent.hpp>
#include <base/components/RigidBodyComponent.hpp>
#include <base/signals/EntityCollisionSignal.hpp>
#include <base/signals/SignalManager.hpp>
#include <raylib.h>
#include <raymath.h>

void PlayerSignalHandler::Init()
{
  auto signalManager = Base::SignalManager::GetInstance();
  signalManager->SubscribeSignal<Base::EntityCollisionSignal>(
    [this](const std::shared_ptr<Base::Signal> &signal) { this->PlayerEnemyCollisionHandler(signal); });
}

void PlayerSignalHandler::PlayerEnemyCollisionHandler(const std::shared_ptr<Base::Signal> event)
{
  auto collEvent = std::static_pointer_cast<Base::EntityCollisionSignal>(event);

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

      if (Vector2Length(collEvent->collisionNormal) > 0)
      {
        impcmp->direction = collEvent->collisionNormal * -1;
      }
      else
      {
        impcmp->direction = rbcmp->direction;
      }
    }
    else if (attack->HasComponent<EnemyTag>())
    {

      Vector2 attackDir = attack->GetComponent<Base::RigidBodyComponent>()->direction;
      auto *impcmp = defence->GetComponent<Base::ImpulseComponent>();

      if (Vector2Length(collEvent->collisionNormal) > 0)
      {
        impcmp->direction = collEvent->collisionNormal * -1;
      }

      impcmp->force = attack->GetComponent<Base::MoveComponent>()->driveForce * 2;
    }
  }
}
