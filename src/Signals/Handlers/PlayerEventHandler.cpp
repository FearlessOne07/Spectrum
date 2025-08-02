#include "PlayerEventHandler.hpp"
#include "Components/BulletComponent.hpp"
#include "Components/DamageComponent.hpp"
#include "Components/EnemyComponent.hpp"
#include "Components/HealthComponent.hpp"
#include "Components/LightCollectorComponent.hpp"
#include "Components/LightComponent.hpp"
#include "Components/ShootComponent.hpp"
#include "Components/Tags/PlayerTag.hpp"
#include "Components/TrackingComponent.hpp"
#include "Scenes/DeathScreen/DeathScreen.hpp"
#include "Signals/EntityDamagedSignal.hpp"
#include "Signals/EntityDiedSignal.hpp"
#include "base/audio/Sound.hpp"
#include "base/components/TransformComponent.hpp"
#include "base/scenes/SceneTransition.hpp"
#include <base/audio/signals/PlaySoundSignal.hpp>
#include <base/components/ImpulseComponent.hpp>
#include <base/components/MoveComponent.hpp>
#include <base/components/RigidBodyComponent.hpp>
#include <base/entities/signals/EntityCollisionSignal.hpp>
#include <base/scenes/SceneLayer.inl>
#include <base/signals/SignalBus.hpp>
#include <memory>
#include <raylib.h>
#include <raymath.h>

void PlayerSignalHandler::Init(Base::SceneLayer *parentLayer)
{
  _parentLayer = parentLayer;
  auto signalManager = Base::SignalBus::GetInstance();

  signalManager->SubscribeSignal<Base::EntityCollisionSignal>(
    [this](const std::shared_ptr<Base::Signal> &signal) { this->PlayerEntityCollisionHandler(signal); });

  signalManager->SubscribeSignal<EntityDiedSignal>(
    [this](const std::shared_ptr<Base::Signal> &signal) { this->PlayerDeathHandler(signal); });

  signalManager->SubscribeSignal<EntityDamagedSignal>(
    [this](const std::shared_ptr<Base::Signal> &signal) { this->PlayerDamaged(signal); });
}

void PlayerSignalHandler::PlayerEntityCollisionHandler(const std::shared_ptr<Base::Signal> event)
{
  auto collEvent = std::static_pointer_cast<Base::EntityCollisionSignal>(event);

  auto attack = collEvent->hittBoxEntity;
  auto defence = collEvent->hurtBoxEntity;

  if (defence->HasComponent<PlayerTag>())
  {
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
      auto dmgcmp = attack->GetComponent<DamageComponent>();
      auto *hlthcmp = defence->GetComponent<HealthComponent>();
      auto *bulcmp = attack->GetComponent<BulletComponent>();
      impcmpdef->direction = attack->GetComponent<Base::RigidBodyComponent>()->direction;

      bus->BroadCastSignal(sig);
      if (!hlthcmp->HealthChanged())
      {
        bus->BroadCastSignal(sig);
        hlthcmp->TakeDamage(dmgcmp->damage);
      }
      auto rbcmp = attack->GetComponent<Base::RigidBodyComponent>();
      impcmpdef->force = bulcmp->sender->GetComponent<ShootComponent>()->bulletKnockbackForce;
      attack->SetDead();
    }
    else if (attack->HasComponent<EnemyComponent>())
    {
      auto *mvcmpAtt = attack->GetComponent<Base::MoveComponent>();
      auto dmgcmp = attack->GetComponent<DamageComponent>();
      auto *hlthcmp = defence->GetComponent<HealthComponent>();
      impcmpdef->direction = attack->GetComponent<TrackingComponent>()->targetDirection;

      if (!hlthcmp->HealthChanged())
      {
        bus->BroadCastSignal(sig);
        hlthcmp->TakeDamage(dmgcmp->damage);
      }
      impcmpdef->force = mvcmpAtt->driveForce * 2;

      auto *impcmpatt = attack->GetComponent<Base::ImpulseComponent>();
      impcmpatt->direction = impcmpdef->direction * -1;
      impcmpatt->force = 0.5 * impcmpdef->force;
    }
    else if (attack->HasComponent<LightComponent>())
    {
      auto lightcolcmp = defence->GetComponent<LightCollectorComponent>();
      auto lightcmp = attack->GetComponent<LightComponent>();
      lightcolcmp->value += lightcmp->value;
      attack->SetDead();
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

void PlayerSignalHandler::PlayerDamaged(const std::shared_ptr<Base::Signal> signal)
{
  auto col = std::static_pointer_cast<EntityDamagedSignal>(signal);

  if (col->entity->HasComponent<PlayerTag>())
  {
    Base::CameraShakeConfig config;
    config.trauma = 0.5;
    config.frequency = 150.0f;
    config.shakeMagnitude = 10.0f;
    config.duration = 1;
    config.traumaMultiplyer = 2;
    config.rotationMagnitude = 4;

    _parentLayer->ShakeCamera(config);
  }
}
