#include "BulletSystem.hpp"
#include "Components/BulletComponent.hpp"
#include "Components/DamageComponent.hpp"
#include "Components/HealthComponent.hpp"
#include "Components/ShootComponent.hpp"
#include "Components/Tags/EnemyTag.hpp"
#include "Components/Tags/PlayerTag.hpp"
#include <base/components/ColliderComponent.hpp>
#include <base/components/ImpulseComponent.hpp>
#include <base/components/MoveComponent.hpp>
#include <base/components/RigidBodyComponent.hpp>
#include <base/components/ShapeComponent.hpp>
#include <base/components/TransformComponent.hpp>
#include <base/entities/Entity.hpp>
#include <base/entities/EntityManager.hpp>
#include <base/signals/EntityCollisionSignal.hpp>
#include <base/signals/Signal.hpp>
#include <base/signals/SignalManager.hpp>
#include <memory>
#include <raymath.h>
#include <vector>

void BulletSystem::Start()
{
  Base::SignalManager::GetInstance()->SubscribeSignal<Base::EntityCollisionSignal>(
    [this](const std::shared_ptr<Base::Signal> &event) -> void { this->EntityCollisionHandler(event); } //
  );
}

void BulletSystem::Update(float dt, Base::EntityManager *entityManager)
{
  std::vector<std::shared_ptr<Base::Entity>> entities_shtcmp = entityManager->Query<ShootComponent>();

  for (auto &e : entities_shtcmp)
  {
    auto *transcmp = e->GetComponent<Base::TransformComponent>();
    auto *shtcmp = e->GetComponent<ShootComponent>();
    auto *dmgcmp = e->GetComponent<DamageComponent>();

    if (shtcmp->bulletFireTimer >= shtcmp->bulletFireRate)
    {
      if (shtcmp->IsFiring)
      {
        shtcmp->bulletFireTimer = 0.f;
        Base::Entity *bullet = entityManager->AddEntity();

        auto *bulTranscmp = bullet->GetComponent<Base::TransformComponent>();
        bulTranscmp->position = transcmp->position;

        auto *rbcmp = bullet->AddComponent<Base::RigidBodyComponent>();
        rbcmp->isKinematic = true;
        rbcmp->speed = shtcmp->bulletSpeed;
        rbcmp->direction = Vector2Subtract(shtcmp->target, transcmp->position);

        auto *mvcmp = bullet->AddComponent<Base::MoveComponent>();

        auto *shpcmp = bullet->AddComponent<Base::ShapeComponent>();
        shpcmp->points = 50;
        shpcmp->color = WHITE;
        shpcmp->radius = 20;
        shpcmp->fill = true;

        auto bulcmp = bullet->AddComponent<BulletComponent>();
        bulcmp->lifeTime = shtcmp->bulletLifetime;
        bulcmp->target = shtcmp->target;
        bulcmp->sender = shtcmp->GetOwner();

        auto abbcmp = bullet->AddComponent<Base::ColliderComponent>();
        abbcmp->radius = shpcmp->radius;
        abbcmp->shape = Base::ColliderComponent::Shape::CIRCLE;
        abbcmp->SetTypeFlag(Base::ColliderComponent::Type::HITBOX);

        auto bulDmgcmp = bullet->AddComponent<DamageComponent>();
        bulDmgcmp->damage = dmgcmp->damage;
      }
    }
    else
    {
      shtcmp->bulletFireTimer += dt;
    }
  }

  // Update Bullets
  std::vector<std::shared_ptr<Base::Entity>> entities_bulcmp = entityManager->Query<BulletComponent>();

  for (auto &e : entities_bulcmp)
  {
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
    attack->HasComponent<BulletComponent>() && defence->HasComponent<EnemyTag>() &&
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
