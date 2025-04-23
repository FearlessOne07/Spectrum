#include "BulletSystem.hpp"
#include "Components/BulletComponent.hpp"
#include "Components/DamageComponent.hpp"
#include "Components/HealthComponent.hpp"
#include "Components/ShootComponent.hpp"
#include "Components/Tags/EnemyTag.hpp"
#include "Components/Tags/PlayerTag.hpp"
#include "base/Entity.hpp"
#include "base/EntityManager.hpp"
#include "base/Event.hpp"
#include "base/EventBus.hpp"
#include "base/components/BoundingBoxComponent.hpp"
#include "base/components/ImpulseComponent.hpp"
#include "base/components/MoveComponent.hpp"
#include "base/components/RigidBodyComponent.hpp"
#include "base/components/ShapeComponent.hpp"
#include "base/components/TransformComponent.hpp"
#include "base/events/EntityCollisionEvent.hpp"
#include "raylib/raymath.h"
#include <memory>
#include <vector>

void BulletSystem::Start()
{
  Base::EventBus::GetInstance()->SubscribeEvent<Base::EntityCollisionEvent>(
    [this](const std::shared_ptr<Base::Event> &event) -> void { this->EntityCollisionHandler(event); } //
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
    auto *impcmpShooter = e->GetComponent<Base::ImpulseComponent>();

    if (shtcmp->bulletFireTimer >= shtcmp->bulletFireRate)
    {
      if (shtcmp->IsFiring)
      {
        shtcmp->bulletFireTimer = 0.f;
        Base::Entity *bullet = entityManager->AddEntity();

        auto *bulTranscmp = bullet->GetComponent<Base::TransformComponent>();
        bulTranscmp->position = transcmp->position;

        auto *rbcmp = bullet->AddComponent<Base::RigidBodyComponent>();
        rbcmp->isKinematic = false;
        rbcmp->mass = 1;

        auto *impcmp = bullet->AddComponent<Base::ImpulseComponent>();
        impcmp->force = shtcmp->bulletForce;
        impcmp->direction = Vector2Subtract(shtcmp->target, transcmp->position);

        auto *mvcmp = bullet->AddComponent<Base::MoveComponent>();

        auto *shpcmp = bullet->AddComponent<Base::ShapeComponent>();
        shpcmp->points = 50;
        shpcmp->color = WHITE;
        shpcmp->radius = 20;
        shpcmp->fill = true;

        auto bulcmp = bullet->AddComponent<BulletComponent>();
        bulcmp->lifeTime = shtcmp->bulletLifetime;
        bulcmp->target = shtcmp->target;

        auto abbcmp = bullet->AddComponent<Base::BoundingBoxComponent>();
        abbcmp->size = {shpcmp->radius * 2, shpcmp->radius * 2};
        abbcmp->positionOffset = {shpcmp->radius, shpcmp->radius};
        abbcmp->SetTypeFlag(Base::BoundingBoxComponent::Type::HITBOX);

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

void BulletSystem::EntityCollisionHandler(const std::shared_ptr<Base::Event> &event)
{
  auto collEvent = std::static_pointer_cast<Base::EntityCollisionEvent>(event);

  auto attack = collEvent->hittBoxEntity;
  auto defence = collEvent->hurtBoxEntity;

  if (attack->HasComponent<BulletComponent>() && defence->HasComponent<EnemyTag>())
  {
    // Bullet
    auto dmgcmp = attack->GetComponent<DamageComponent>();
    attack->SetDead();

    // Enemy
    auto hlthcmp = defence->GetComponent<HealthComponent>();
    hlthcmp->health -= dmgcmp->damage;
  }
}
