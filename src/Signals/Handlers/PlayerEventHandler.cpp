#include "PlayerEventHandler.hpp"
#include "Components/BulletComponent.hpp"
#include "Components/DamageComponent.hpp"
#include "Components/HealthComponent.hpp"
#include "Components/ShootComponent.hpp"
#include "Components/Tags/EnemyTag.hpp"
#include "Components/Tags/PlayerTag.hpp"
#include "Scenes/DeathScreen/DeathScreen.hpp"
#include "Signals/EntityDiedSignal.hpp"
#include "base/scenes/Scene.hpp"
#include "base/scenes/SceneTransition.hpp"
#include <base/components/ImpulseComponent.hpp>
#include <base/components/MoveComponent.hpp>
#include <base/components/RigidBodyComponent.hpp>
#include <base/signals/EntityCollisionSignal.hpp>
#include <base/signals/SignalBus.hpp>
#include <iostream>
#include <memory>
#include <raylib.h>
#include <raymath.h>

void PlayerSignalHandler::Init(Base::Scene *scene)
{
  _scene = scene;
  auto signalManager = Base::SignalBus::GetInstance();

  signalManager->SubscribeSignal<Base::EntityCollisionSignal>(
    [this](const std::shared_ptr<Base::Signal> &signal) { this->PlayerEnemyCollisionHandler(signal); });

  signalManager->SubscribeSignal<EntityDiedSignal>(
    [this](const std::shared_ptr<Base::Signal> &signal) { this->PlayerDeathHandler(signal); });
}

void PlayerSignalHandler::PlayerEnemyCollisionHandler(const std::shared_ptr<Base::Signal> event)
{
  auto collEvent = std::static_pointer_cast<Base::EntityCollisionSignal>(event);

  auto attack = collEvent->hittBoxEntity;
  auto defence = collEvent->hurtBoxEntity;

  if (defence->HasComponent<PlayerTag>())
  {
    Base::CameraManager::CameraShakeConfig config;
    config.trauma = 0.5;
    config.frequency = 120.0f;
    config.shakeMagnitude = 10.0f;
    config.duration = 1;
    config.traumaMultiplyer = 2;
    config.rotationMagnitude = 4;

    if (                                                                        //
      attack->HasComponent<BulletComponent>() &&                                //
      attack->GetComponent<BulletComponent>()->sender->HasComponent<EnemyTag>() //
    )
    {
      _scene->GetCameraManager()->Shake(config);
      auto dmgcmp = attack->GetComponent<DamageComponent>();
      auto *hlthcmp = defence->GetComponent<HealthComponent>();

      if (!hlthcmp->hasPendingSickness)
      {
        hlthcmp->hasPendingSickness = true;
        hlthcmp->sickness += dmgcmp->damage;
      }

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
      _scene->GetCameraManager()->Shake(config);
      auto dmgcmp = attack->GetComponent<DamageComponent>();
      auto *hlthcmp = defence->GetComponent<HealthComponent>();

      if (!hlthcmp->hasPendingSickness)
      {
        hlthcmp->hasPendingSickness = true;
        hlthcmp->sickness += dmgcmp->damage;
      }

      Vector2 attackDir = attack->GetComponent<Base::RigidBodyComponent>()->direction;
      auto *impcmp = defence->GetComponent<Base::ImpulseComponent>();

      if (Vector2Length(collEvent->collisionNormal) > 0)
      {
        impcmp->direction = collEvent->collisionNormal;
      }

      impcmp->force = attack->GetComponent<Base::MoveComponent>()->driveForce * 2;
    }
  }
}

void PlayerSignalHandler::PlayerDeathHandler(const std::shared_ptr<Base::Signal> signal)
{
  if (auto entityDiedSig = std::dynamic_pointer_cast<EntityDiedSignal>(signal))
  {
    if (entityDiedSig->entity->HasComponent<PlayerTag>())
    {
      _scene->SetSceneTransition<DeathScreen>(Base::SceneRequest::REPLACE_CURRENT_SCENE);
    }
  }
}
