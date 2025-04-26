#include "Spawner.hpp"
#include "Components/DamageComponent.hpp"
#include "Components/HealthComponent.hpp"
#include "Components/ShootComponent.hpp"
#include "Components/Tags/EnemyTag.hpp"
#include "Components/Tags/PlayerTag.hpp"
#include "Components/TrackingComponent.hpp"
#include "Components/TransformEffects.hpp"
#include <base/AssetManager.hpp>
#include <base/Entity.hpp>
#include <base/EntityManager.hpp>
#include <base/RenderContext.hpp>
#include <base/RenderContextSingleton.hpp>
#include <base/components/ColliderComponent.hpp>
#include <base/components/ImpulseComponent.hpp>
#include <base/components/InputComponent.hpp>
#include <base/components/MoveComponent.hpp>
#include <base/components/RigidBodyComponent.hpp>
#include <base/components/ShapeComponent.hpp>
#include <base/components/TransformComponent.hpp>
#include <cstddef>
#include <random>
#include <raylib.h>
#include <raylib/raylib.h>

void Spawner::SetToSpawn(std::vector<EnemyType> toSpawn)
{
  for (EnemyType type : toSpawn)
  {
    _toSpawn.push(type);
  }
}

int Spawner::GetToSpawnCount() const
{
  return _toSpawn.size();
}

size_t Spawner::SpawnPlayer(Base::EntityManager *entityManager, Base::AssetManager *assetManager, Vector2 position)
{
  Base::Entity *e = entityManager->AddEntity();
  const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();

  auto *transcmp = e->GetComponent<Base::TransformComponent>();
  transcmp->position = position;

  auto *transfxcmp = e->AddComponent<TransformEffectsComponent>();
  transfxcmp->targetAngularVelocity = 180;
  transfxcmp->angularAcceleration = 1;
  transfxcmp->bind = true;
  transfxcmp->bindMin = GetScreenToWorld2D({0, 0}, rd->camera);
  transfxcmp->bindMax = GetScreenToWorld2D({rd->gameWidth, rd->gameHeight}, rd->camera);

  auto *mvcmp = e->AddComponent<Base::MoveComponent>();
  mvcmp->driveForce = 3000;
  mvcmp->brakeForce = 500.f;

  auto rbcmp = e->AddComponent<Base::RigidBodyComponent>();
  rbcmp->isKinematic = false;
  rbcmp->mass = 1;
  rbcmp->drag = 3;

  auto *shtcmp = e->AddComponent<ShootComponent>();
  shtcmp->bulletFireRate = 0.6;
  shtcmp->bulletLifetime = 3;
  shtcmp->bulletFireTimer = 1;
  shtcmp->bulletSpeed = 1500.f;

  auto *shpcmp = e->AddComponent<Base::ShapeComponent>();
  shpcmp->fill = true;
  shpcmp->color = WHITE;
  shpcmp->points = 8;
  shpcmp->radius = 30;

  auto *abbcmp = e->AddComponent<Base::ColliderComponent>();
  abbcmp->radius = shpcmp->radius;
  abbcmp->shape = Base::ColliderComponent::Shape::CIRCLE;
  abbcmp->SetTypeFlag(Base::ColliderComponent::Type::HURTBOX);

  auto *inpcmp = e->AddComponent<Base::InputComponent>();
  inpcmp->BindKeyDown(KEY_A, [rbcmp]() { rbcmp->direction.x = -1; });
  inpcmp->BindKeyDown(KEY_D, [rbcmp]() { rbcmp->direction.x = 1; });
  inpcmp->BindKeyDown(KEY_W, [rbcmp]() { rbcmp->direction.y = -1; });
  inpcmp->BindKeyDown(KEY_S, [rbcmp]() { rbcmp->direction.y = 1; });

  inpcmp->BindKeyReleased(KEY_A, [rbcmp]() { rbcmp->direction.x = 0; });
  inpcmp->BindKeyReleased(KEY_D, [rbcmp]() { rbcmp->direction.x = 0; });
  inpcmp->BindKeyReleased(KEY_S, [rbcmp]() { rbcmp->direction.y = 0; });
  inpcmp->BindKeyReleased(KEY_W, [rbcmp]() { rbcmp->direction.y = 0; });

  inpcmp->BindMouseButtonDown(MOUSE_BUTTON_LEFT, [shtcmp]() {
    const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();
    shtcmp->IsFiring = true;
    shtcmp->target = GetScreenToWorld2D(rd->GetScreenToGame(GetMousePosition()), rd->camera);
  });
  inpcmp->BindMouseButtonReleased(MOUSE_BUTTON_LEFT, [shtcmp]() { shtcmp->IsFiring = false; });

  auto dmgcmp = e->AddComponent<DamageComponent>();
  dmgcmp->damage = 2;
  e->AddComponent<Base::ImpulseComponent>();
  e->AddComponent<PlayerTag>();

  _playerID = e->GetID();
  return _playerID;
}

void Spawner::SpawnWave(float dt, Base::EntityManager *entityManager, size_t playerID)
{

  if (_spawnTimer >= _spawnDuration && !_toSpawn.empty())
  {
    _spawnTimer = 0.f;
    const Base::RenderContext *rctx = Base::RenderContextSingleton::GetInstance();

    EnemyType &type = _toSpawn.front();
    _toSpawn.pop();

    std::random_device rd;
    std::mt19937_64 gen(rd());

    std::uniform_int_distribution sideDist(1, 4);
    int side = sideDist(gen);

    Vector2 min = {-_spawnOffset, -_spawnOffset};
    Vector2 max = {rctx->gameWidth + _spawnOffset, rctx->gameHeight + _spawnOffset};

    Vector2 position = {0, 0};
    float ypos = 0.f;
    float xpos = 0.f;

    switch (side)
    {
    case 1: // Right
      ypos = std::uniform_real_distribution<float>(min.y, max.y)(gen);
      position = GetScreenToWorld2D({max.x, ypos}, rctx->camera);
      break;
    case 2: // Left
      ypos = std::uniform_real_distribution<float>(min.y, max.y)(gen);
      position = GetScreenToWorld2D({min.x, ypos}, rctx->camera);
      break;
    case 3: // Up
      xpos = std::uniform_real_distribution<float>(min.x, max.x)(gen);
      position = GetScreenToWorld2D({xpos, min.y}, rctx->camera);
      break;
    case 4: // Down
      xpos = std::uniform_real_distribution<float>(min.x, max.x)(gen);
      position = GetScreenToWorld2D({xpos, max.y}, rctx->camera);
      break;
    }

    Base::Entity *e = entityManager->AddEntity();

    e->AddComponent<EnemyTag>();

    auto *transcmp = e->GetComponent<Base::TransformComponent>();
    transcmp->position = position;

    auto *trckcmp = e->AddComponent<TrackingComponent>();
    trckcmp->targetEntityID = playerID;

    auto *mvcmp = e->AddComponent<Base::MoveComponent>();
    mvcmp->driveForce = std::uniform_int_distribution(300, 500)(gen);

    auto *rbcmp = e->AddComponent<Base::RigidBodyComponent>();
    rbcmp->isKinematic = false;
    rbcmp->drag = 3;
    rbcmp->mass = 1;

    auto *shpcmp = e->AddComponent<Base::ShapeComponent>();
    shpcmp->fill = true;
    shpcmp->fillOutline = true;
    shpcmp->outlineColor = WHITE;
    shpcmp->radius = 30;

    auto *abbcmp = e->AddComponent<Base::ColliderComponent>();
    abbcmp->radius = shpcmp->radius;
    abbcmp->shape = Base::ColliderComponent::Shape::CIRCLE;
    abbcmp->SetTypeFlag(Base::ColliderComponent::Type::HURTBOX);
    abbcmp->SetTypeFlag(Base::ColliderComponent::Type::HITBOX);

    auto hlthcmp = e->AddComponent<HealthComponent>();
    hlthcmp->health = 8;

    auto dmgcmp = e->AddComponent<DamageComponent>();
    dmgcmp->damage = 2;

    auto transfxcmp = e->AddComponent<TransformEffectsComponent>();
    transfxcmp->rotate = true;
    transfxcmp->angularAcceleration = 0.5;
    transfxcmp->targetAngularVelocity = 90;
    transfxcmp->bind = false;

    switch (type)
    {
    case EnemyType::CHASER:
      shpcmp->points = 3;
      shpcmp->color = RED;
      break;
    case EnemyType::SHOOTER:
      shpcmp->points = 6;
      shpcmp->color = BLUE;
      trckcmp->trackingDistance = 800;

      auto *shtcmp = e->AddComponent<ShootComponent>();
      shtcmp->bulletFireRate = 2.f;
      shtcmp->bulletKnockbackForce = 800;
      shtcmp->bulletSpeed = 1000.f;

      transfxcmp->bindMin = GetScreenToWorld2D({0, 0}, rctx->camera);
      transfxcmp->bindMax = GetScreenToWorld2D({rctx->gameWidth, rctx->gameHeight}, rctx->camera);
      break;
    }
  }
  else
  {
    _spawnTimer += dt;
  }
}
