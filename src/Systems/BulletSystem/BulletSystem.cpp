#include "BulletSystem.hpp"
#include "Components/BulletComponent.hpp"
#include "Components/DamageComponent.hpp"
#include "Components/EnemyComponent.hpp"
#include "Components/HealthComponent.hpp"
#include "Components/ShootComponent.hpp"
#include "Components/Tags/PlayerTag.hpp"
#include "base/components/SpriteComponent.hpp"
#include "base/scenes/Scene.hpp"
#include <base/audio/signals/PlaySoundSignal.hpp>
#include <base/components/ColliderComponent.hpp>
#include <base/components/ImpulseComponent.hpp>
#include <base/components/MoveComponent.hpp>
#include <base/components/RigidBodyComponent.hpp>
#include <base/components/ShapeComponent.hpp>
#include <base/components/TransformComponent.hpp>
#include <base/entities/Entity.hpp>
#include <base/entities/EntityManager.hpp>
#include <base/entities/signals/EntityCollisionSignal.hpp>
#include <base/signals/Signal.hpp>
#include <base/signals/SignalBus.hpp>
#include <memory>
#include <raymath.h>

void BulletSystem::Start()
{
  Base::SignalBus::GetInstance()->SubscribeSignal<Base::EntityCollisionSignal>(
    [this](const std::shared_ptr<Base::Signal> &event) -> void { this->EntityCollisionHandler(event); } //
  );
}

void BulletSystem::Update(float dt, Base::EntityManager *entityManager, const Base::Scene *currentScene)
{
  auto entities_shtcmp = entityManager->Query<ShootComponent>();

  for (auto &item : entities_shtcmp)
  {
    auto e = item->item;
    auto *transcmp = e->GetComponent<Base::TransformComponent>();
    auto *shtcmp = e->GetComponent<ShootComponent>();
    auto *dmgcmp = e->GetComponent<DamageComponent>();

    if (shtcmp->bulletFireTimer >= shtcmp->bulletFireRate)
    {
      if (shtcmp->IsFiring)
      {
        shtcmp->bulletFireTimer = 0.f;
        auto bullet = entityManager->CreateEntity();

        auto *bulTranscmp = bullet->GetComponent<Base::TransformComponent>();
        bulTranscmp->position = transcmp->position;

        auto *rbcmp = bullet->AddComponent<Base::RigidBodyComponent>();
        rbcmp->isKinematic = true;
        rbcmp->speed = shtcmp->bulletSpeed;
        rbcmp->direction = Vector2Subtract(shtcmp->target, transcmp->position);

        auto *mvcmp = bullet->AddComponent<Base::MoveComponent>();
        auto sprtcmp = bullet->AddComponent<Base::SpriteComponent>(                //
          shtcmp->bulletTexture, Vector2{0, 0}, Vector2{200, 200}, Vector2{32, 32} //
        );

        auto bulcmp = bullet->AddComponent<BulletComponent>();
        bulcmp->lifeTime = shtcmp->bulletLifetime;
        bulcmp->target = shtcmp->target;
        bulcmp->sender = shtcmp->GetOwner();

        auto abbcmp = bullet->AddComponent<Base::ColliderComponent>();
        abbcmp->radius = ((sprtcmp->GetTexture().Get()->GetRaylibTexture()->width - 10) / 2.f) *
                         (sprtcmp->GetTargetSize().x / sprtcmp->GetTextureSourceRect().width);
        abbcmp->shape = Base::ColliderComponent::Shape::CIRCLE;
        abbcmp->SetTypeFlag(Base::ColliderComponent::Type::HITBOX);

        auto bulDmgcmp = bullet->AddComponent<DamageComponent>();
        bulDmgcmp->damage = dmgcmp->damage;
        entityManager->AddEntity(bullet);

        // Emmit sound signal
        auto bus = Base::SignalBus::GetInstance();
        std::shared_ptr<Base::PlaySoundSignal> sig = std::make_shared<Base::PlaySoundSignal>();
        sig->soundHandle = currentScene->GetAsset<Base::Sound>("bullet-fire");
        sig->soundVolume = 0.5;
        bus->BroadCastSignal(sig);
      }
    }
    else
    {
      shtcmp->bulletFireTimer += dt;
    }
  }

  // Update Bullets
  auto entities_bulcmp = entityManager->Query<BulletComponent>();

  for (auto &item : entities_bulcmp)
  {
    auto e = item->item;
    if (e)
    {
      auto bulcmp = e->GetComponent<BulletComponent>();

      if (bulcmp->lifeTimer > bulcmp->lifeTime)
      {
        e->SetDead();
      }
      else
      {
        bulcmp->lifeTimer += dt;
      }
    }
  }
}

void BulletSystem::EntityCollisionHandler(const std::shared_ptr<Base::Signal> &event)
{
  auto collEvent = std::static_pointer_cast<Base::EntityCollisionSignal>(event);

  auto attack = collEvent->hittBoxEntity;
  auto defence = collEvent->hurtBoxEntity;

  if ( //
    attack->HasComponent<BulletComponent>() && defence->HasComponent<EnemyComponent>() &&
    attack->GetComponent<BulletComponent>()->sender->HasComponent<PlayerTag>() //
  )
  {
    // Bullet
    auto dmgcmp = attack->GetComponent<DamageComponent>();
    attack->SetDead();

    // Enemy
    auto hlthcmp = defence->GetComponent<HealthComponent>();
    hlthcmp->health -= dmgcmp->damage;
  }
}
