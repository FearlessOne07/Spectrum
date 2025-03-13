#include "BulletSystem.hpp"
#include "Components/BulletComponent/BulletComponent.hpp"
#include "Components/ShootComponent/ShootComponent.hpp"
#include "base/Entity.hpp"
#include "base/EntityManager.hpp"
#include "base/components/MoveComponent.hpp"
#include "base/components/ShapeComponent.hpp"
#include "base/components/TransformComponent.hpp"
#include "raylib/raymath.h"
#include <memory>
#include <vector>

void BulletSystem::Update(float dt, Base::EntityManager *entityManager)
{
  std::vector<std::shared_ptr<Base::Entity>> entities_shtcmp = entityManager->Query<ShootComponent>();

  for (auto &e : entities_shtcmp)
  {
    auto *transcmp = e->GetComponent<Base::TransformComponent>();
    auto *shtcmp = e->GetComponent<ShootComponent>();

    if (shtcmp->bulletFireTimer >= shtcmp->bulletFireRate)
    {
      if (shtcmp->IsFiring)
      {
        shtcmp->bulletFireTimer = 0.f;
        shtcmp->IsFiring = false;

        Base::Entity *bullet = entityManager->AddEntity();

        auto *bulTranscmp = bullet->GetComponent<Base::TransformComponent>();
        bulTranscmp->position = transcmp->position;

        auto *mvcmp = bullet->AddComponent<Base::MoveComponent>();
        mvcmp->targetVelocity = Vector2Subtract(shtcmp->target, transcmp->position);
        mvcmp->speed = shtcmp->bulletSpeed;
        mvcmp->acceleration = shtcmp->bulletSpeed;

        auto *shpcmp = bullet->AddComponent<Base::ShapeComponent>();
        shpcmp->points = 50;
        shpcmp->color = WHITE;
        shpcmp->radius = 20;
        shpcmp->fill = true;

        auto bulcmp = bullet->AddComponent<BulletComponent>();
        bulcmp->lifeTime = shtcmp->bulletLifetime;
        bulcmp->target = shtcmp->target;
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
