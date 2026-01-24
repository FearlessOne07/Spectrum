#include "EntityEventHandler.hpp"
#include "Scenes/GameScene/SharedGameData.hpp"
#include "Components/BulletComponent.hpp"
#include "Components/DamageComponent.hpp"
#include "Components/EnemyComponent.hpp"
#include "Components/HealthComponent.hpp"
#include "Components/KnockBackComponent.hpp"
#include "Components/LightCollectorComponent.hpp"
#include "Components/LightComponent.hpp"
#include "Components/ShootComponent.hpp"
#include "Components/Tags/ExplosionTag.hpp"
#include "Components/Tags/HealthPack.hpp"
#include "Components/Tags/PlayerTag.hpp"
#include "Scenes/DeathScreen/DeathScreen.hpp"
#include "ShaderEffects/Vignette/Vignette.hpp"
#include "Signals/EntityDamagedSignal.hpp"
#include "Signals/EntityDiedSignal.hpp"
#include "base/audio/Sound.hpp"
#include "base/components/ColliderComponent.hpp"
#include "base/components/TransformComponent.hpp"
#include "base/entities/EntityManager.hpp"
#include "base/scenes/SceneTransition.hpp"
#include "base/util/Circle.hpp"
#include "base/util/Math.hpp"
#include <base/audio/signals/PlaySoundSignal.hpp>
#include <base/components/ImpulseComponent.hpp>
#include <base/components/MoveComponent.hpp>
#include <base/components/RigidBodyComponent.hpp>
#include <base/entities/signals/EntityCollisionSignal.hpp>
#include <base/signals/SignalBus.hpp>
#include <memory>

void EntitySignalHandler::Init(Base::SceneLayer *parentLayer)
{
  _parentLayer = parentLayer;
  auto signalManager = Base::SignalBus::GetInstance();

  signalManager->SubscribeSignal<Base::EntityCollisionSignal>(
    [this](const std::shared_ptr<Base::Signal> &signal) { this->CollisionHandler(signal); });

  signalManager->SubscribeSignal<EntityDiedSignal>(
    [this](const std::shared_ptr<Base::Signal> &signal) { this->DeathHandler(signal); });

  signalManager->SubscribeSignal<EntityDamagedSignal>(
    [this](const std::shared_ptr<Base::Signal> &signal) { this->DamagedHandler(signal); });
}

void EntitySignalHandler::CollisionHandler(const std::shared_ptr<Base::Signal> event)
{
  auto collEvent = std::static_pointer_cast<Base::EntityCollisionSignal>(event);

  auto attack = collEvent->hittBoxEntity;
  auto defence = collEvent->hurtBoxEntity;

  if (defence->HasComponent<PlayerTag>())
  {
    Base::Vector2 knockBackDir = {0, 0};
    auto transAtt = attack->GetComponent<Base::TransformComponent>();
    auto transDef = defence->GetComponent<Base::TransformComponent>();
    auto impcmpdef = defence->GetComponent<Base::ImpulseComponent>();

    auto bus = Base::SignalBus::GetInstance();
    std::shared_ptr<Base::PlaySoundSignal> sig = std::make_shared<Base::PlaySoundSignal>();
    sig->soundHandle = _parentLayer->GetOwner()->Engine().Assets->GetAsset<Base::Sound>("player-hit");
    sig->soundVolume = 0.75;

    if (                                                                                                   //
      attack->HasComponent<BulletComponent>() &&                                                           //
      attack->GetComponent<BulletComponent>()->sender->HasComponent<EnemyComponent>() && attack->IsAlive() //
    )
    {
      auto dmgcmp = attack->GetComponent<DamageComponent>();
      auto hlthcmp = defence->GetComponent<HealthComponent>();
      auto bulcmp = attack->GetComponent<BulletComponent>();
      impcmpdef->direction = attack->GetComponent<Base::RigidBodyComponent>()->direction;

      bus->BroadCastSignal(sig);
      if (!hlthcmp->TookDamage())
      {
        bus->BroadCastSignal(sig);
        hlthcmp->TakeDamage(dmgcmp->GetDamage());
      }
      auto rbcmp = attack->GetComponent<Base::RigidBodyComponent>();
      impcmpdef->force = bulcmp->sender->GetComponent<ShootComponent>()->bulletKnockbackForce;
      attack->SetDead();
    }
    else if (attack->HasComponent<EnemyComponent>())
    {
      auto enemycmp = attack->GetComponent<EnemyComponent>();

      if (enemycmp->type == EnemyType::KAMIKAZE)
      {
        attack->GetComponent<HealthComponent>()->Kill();
      }

      auto mvcmpAtt = attack->GetComponent<Base::MoveComponent>();
      auto dmgcmp = attack->GetComponent<DamageComponent>();
      auto hlthcmp = defence->GetComponent<HealthComponent>();

      Base::Vector2 attPosition = attack->GetComponent<Base::TransformComponent>()->position;
      Base::Vector2 defPosition = defence->GetComponent<Base::TransformComponent>()->position;
      impcmpdef->direction = Base::Math::Normalize(defPosition - attPosition);

      if (!hlthcmp->TookDamage())
      {
        bus->BroadCastSignal(sig);
        hlthcmp->TakeDamage(dmgcmp->GetDamage());
      }
      auto impcmpatt = attack->GetComponent<Base::ImpulseComponent>();
      impcmpatt->direction = impcmpdef->direction * -1.f;
      impcmpatt->force = mvcmpAtt->driveForce * 1.1;

      if (attack->HasComponent<KnockBackComponent>())
      {
        auto kckbckcmp = attack->GetComponent<KnockBackComponent>();
        impcmpdef->force = kckbckcmp->GetForce();
      }
    }
    else if (attack->HasComponent<LightComponent>())
    {
      auto lightcolcmp = defence->GetComponent<LightCollectorComponent>();
      auto lightcmp = attack->GetComponent<LightComponent>();
      lightcolcmp->value += lightcmp->value;
      attack->SetDead();
    }
    else if (attack->HasComponent<HealthPack>())
    {
      auto healthpck = attack->GetComponent<HealthPack>();
      auto healthcmp = defence->GetComponent<HealthComponent>();
      healthcmp->SetHealth(healthcmp->GetHealth() + (healthcmp->GetMaxHealth() * healthpck->restorationPercentage));
      attack->SetDead();

      auto vignette = _parentLayer->GetShaderEffect<Vignette>();
      vignette->SetVignetteColor(Base::Color{73, 162, 105, 255});
      vignette->Flash();
    }
    else if (attack->HasComponent<ExplosionTag>())
    {
      auto expcmp = attack->GetComponent<ExplosionTag>();
      if (!expcmp->exploded)
      {
        auto colcmp = attack->GetComponent<Base::ColliderComponent>();
        auto entities = _parentLayer->GetOwner()->Engine().Entities->QueryArea(Base::Circle{
          transAtt->position,
          colcmp->radius,
        });

        for (auto &e : entities)
        {
          if (e->item == attack)
          {
            continue;
          }

          Base::Vector2 playerPos = e->item->GetComponent<Base::TransformComponent>()->position;
          Base::Vector2 direction = Base::Math::Normalize(playerPos - transAtt->position);

          if (e->item->HasComponent<Base::ImpulseComponent>())
          {
            auto playerImpcmp = e->item->GetComponent<Base::ImpulseComponent>();
            playerImpcmp->direction = direction;
            playerImpcmp->force = expcmp->explosionForce;
          }
        }
      }
    }
  }
}

void EntitySignalHandler::DeathHandler(const std::shared_ptr<Base::Signal> signal)
{
  if (auto entityDiedSig = std::dynamic_pointer_cast<EntityDiedSignal>(signal))
  {
    if (entityDiedSig->entity->HasComponent<PlayerTag>())
    {
      Base::SceneData data;
      data.Set(_parentLayer->GetOwner()->GetSharedData<SharedGameData>()->PlayerShip);
      _parentLayer->GetOwner()->SetSceneTransition<DeathScreen>(Base::SceneRequest::ReplaceCurrentScene, data);
    }
  }
}

void EntitySignalHandler::DamagedHandler(const std::shared_ptr<Base::Signal> signal)
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
