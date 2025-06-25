#include "PlayerEventHandler.hpp"
#include "Components/BulletComponent.hpp"
#include "Components/DamageComponent.hpp"
#include "Components/EnemyComponent.hpp"
#include "Components/HealthComponent.hpp"
#include "Components/ShootComponent.hpp"
#include "Components/Tags/PlayerTag.hpp"
#include "Components/TrackingComponent.hpp"
#include "Scenes/DeathScreen/DeathScreen.hpp"
#include "Signals/EntityDiedSignal.hpp"
#include "base/audio/Sound.hpp"
#include "base/components/TransformComponent.hpp"
#include "base/scenes/SceneLayer.inl"
#include "base/scenes/SceneTransition.hpp"
#include <base/audio/signals/PlaySoundSignal.hpp>
#include <base/components/ImpulseComponent.hpp>
#include <base/components/MoveComponent.hpp>
#include <base/components/RigidBodyComponent.hpp>
#include <base/entities/signals/EntityCollisionSignal.hpp>
#include <base/signals/SignalBus.hpp>
#include <memory>
#include <raylib.h>
#include <raymath.h>

void PlayerSignalHandler::Init(Base::SceneLayer *parentLayer)
{
  _parentLayer = parentLayer;
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
    Base::CameraShakeConfig config;
    config.trauma = 0.5;
    config.frequency = 150.0f;
    config.shakeMagnitude = 6.0f;
    config.duration = 1;
    config.traumaMultiplyer = 2;
    config.rotationMagnitude = 4;

    Vector2 knockBackDir = {0, 0};
    auto *transAtt = attack->GetComponent<Base::TransformComponent>();
    auto *transDef = defence->GetComponent<Base::TransformComponent>();

    auto impcmpdef = defence->GetComponent<Base::ImpulseComponent>();

    auto bus = Base::SignalBus::GetInstance();
    std::shared_ptr<Base::PlaySoundSignal> sig = std::make_shared<Base::PlaySoundSignal>();
    sig->soundHandle = _parentLayer->GetAsset<Base::Sound>("player-hit");
    sig->soundVolume = 0.75;

    if (                                                                              //
      attack->HasComponent<BulletComponent>() &&                                      //
      attack->GetComponent<BulletComponent>()->sender->HasComponent<EnemyComponent>() //
    )
    {
      _parentLayer->ShakeCamera(config);
      auto dmgcmp = attack->GetComponent<DamageComponent>();
      auto *hlthcmp = defence->GetComponent<HealthComponent>();
      auto *bulcmp = attack->GetComponent<BulletComponent>();
      impcmpdef->direction = attack->GetComponent<Base::RigidBodyComponent>()->direction;

      if (!hlthcmp->hasPendingSickness)
      {
        bus->BroadCastSignal(sig);
        hlthcmp->hasPendingSickness = true;
        hlthcmp->sickness += dmgcmp->damage;
      }
      auto rbcmp = attack->GetComponent<Base::RigidBodyComponent>();
      impcmpdef->force = bulcmp->sender->GetComponent<ShootComponent>()->bulletKnockbackForce;
      attack->SetDead();
    }
    else if (attack->HasComponent<EnemyComponent>())
    {
      _parentLayer->ShakeCamera(config);
      auto *mvcmpAtt = attack->GetComponent<Base::MoveComponent>();
      auto dmgcmp = attack->GetComponent<DamageComponent>();
      auto *hlthcmp = defence->GetComponent<HealthComponent>();
      impcmpdef->direction = attack->GetComponent<TrackingComponent>()->targetDirection;

      if (!hlthcmp->hasPendingSickness)
      {
        bus->BroadCastSignal(sig);
        hlthcmp->hasPendingSickness = true;
        hlthcmp->sickness += dmgcmp->damage;
      }
      impcmpdef->force = mvcmpAtt->driveForce * 2;

      auto *impcmpatt = attack->GetComponent<Base::ImpulseComponent>();
      impcmpatt->direction = impcmpdef->direction * -1;
      impcmpatt->force = 0.5 * impcmpdef->force;
    }
  }
}

void PlayerSignalHandler::PlayerDeathHandler(const std::shared_ptr<Base::Signal> signal)
{
  if (auto entityDiedSig = std::dynamic_pointer_cast<EntityDiedSignal>(signal))
  {
    if (entityDiedSig->entity->HasComponent<PlayerTag>())
    {
      _parentLayer->SetSceneTransition<DeathScreen>(Base::SceneRequest::REPLACE_CURRENT_SCENE);
    }
  }
}
