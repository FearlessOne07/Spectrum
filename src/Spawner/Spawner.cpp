#include "Spawner.hpp"
#include "Components/DamageComponent.hpp"
#include "Components/HealthComponent.hpp"
#include "Components/ShootComponent.hpp"
#include "Components/Tags/EnemyTag.hpp"
#include "Components/Tags/PlayerTag.hpp"
#include "Components/TrackingComponent.hpp"
#include "Components/TransformEffects.hpp"
#include "Signals/PlayerSpawnedSignal.hpp"
#include "base/camera/CameraManager.hpp"
#include "base/components/TextureComponent.hpp"
#include "base/signals/SignalBus.hpp"
#include <base/assets/AssetManager.hpp>
#include <base/components/ColliderComponent.hpp>
#include <base/components/ImpulseComponent.hpp>
#include <base/components/InputComponent.hpp>
#include <base/components/MoveComponent.hpp>
#include <base/components/RigidBodyComponent.hpp>
#include <base/components/ShapeComponent.hpp>
#include <base/components/TransformComponent.hpp>
#include <base/entities/Entity.hpp>
#include <base/entities/EntityManager.hpp>
#include <base/game/RenderContext.hpp>
#include <base/game/RenderContextSingleton.hpp>
#include <cstddef>
#include <memory>
#include <random>
#include <raylib.h>

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

size_t Spawner::SpawnPlayer( //
  Base::EntityManager *entityManager, Base::AssetManager *assetManager, Base::CameraManager *camManager,
  Vector2 position //
)
{
  Base::Entity *e = entityManager->AddEntity();
  const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();

  auto *transcmp = e->GetComponent<Base::TransformComponent>();
  transcmp->position = position;

  auto *transfxcmp = e->AddComponent<TransformEffectsComponent>();
  transfxcmp->targetAngularVelocity = 180;
  transfxcmp->angularAcceleration = 1;
  transfxcmp->bind = true;
  transfxcmp->bindMin = camManager->GetScreenToWorld({0, 0});
  transfxcmp->bindMax = camManager->GetScreenToWorld({rd->gameWidth, rd->gameHeight});
  transfxcmp->rotate = true;

  auto *mvcmp = e->AddComponent<Base::MoveComponent>();
  mvcmp->driveForce = 3000;
  mvcmp->brakeForce = 500.f;

  auto rbcmp = e->AddComponent<Base::RigidBodyComponent>();
  rbcmp->isKinematic = false;
  rbcmp->mass = 1;
  rbcmp->drag = 3;

  auto hlthcmp = e->AddComponent<HealthComponent>();
  hlthcmp->health = 20;

  auto *shtcmp = e->AddComponent<ShootComponent>();
  shtcmp->bulletFireRate = 0.6;
  shtcmp->bulletLifetime = 3;
  shtcmp->bulletFireTimer = 1;
  shtcmp->bulletSpeed = 1500.f;

  auto txtcmp = e->AddComponent<Base::TextureComponent>();
  txtcmp->texture = assetManager->GetAsset<Texture>("ship");
  txtcmp->targetSize = {64, 64};
  txtcmp->source = {0, 0, static_cast<float>(txtcmp->texture->width), static_cast<float>(txtcmp->texture->height)};

  auto *abbcmp = e->AddComponent<Base::ColliderComponent>();
  abbcmp->radius = 32;
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

  inpcmp->BindMouseButtonDown(MOUSE_BUTTON_LEFT, [shtcmp, camManager]() {
    const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();
    shtcmp->IsFiring = true;
    shtcmp->target = camManager->GetScreenToWorld(rd->mousePosition);
  });
  inpcmp->BindMouseButtonReleased(MOUSE_BUTTON_LEFT, [shtcmp]() { shtcmp->IsFiring = false; });

  auto dmgcmp = e->AddComponent<DamageComponent>();
  dmgcmp->damage = 2;
  e->AddComponent<Base::ImpulseComponent>();
  e->AddComponent<PlayerTag>();

  _playerID = e->GetID();

  auto bus = Base::SignalBus::GetInstance();
  std::shared_ptr<PlayerSpawnedSignal> sig = std::make_shared<PlayerSpawnedSignal>();
  sig->player = e;
  bus->BroadCastSignal(sig);
  return _playerID;
}

void Spawner::SpawnWave(float dt, Base::EntityManager *entityManager, Base::CameraManager *camManager, size_t playerID)
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
      position = camManager->GetScreenToWorld({max.x, ypos});
      break;
    case 2: // Left
      ypos = std::uniform_real_distribution<float>(min.y, max.y)(gen);
      position = camManager->GetScreenToWorld({min.x, ypos});
      break;
    case 3: // Up
      xpos = std::uniform_real_distribution<float>(min.x, max.x)(gen);
      position = camManager->GetScreenToWorld({xpos, min.y});
      break;
    case 4: // Down
      xpos = std::uniform_real_distribution<float>(min.x, max.x)(gen);
      position = camManager->GetScreenToWorld({xpos, max.y});
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
      trckcmp->trackingDistance = 1000;

      auto *shtcmp = e->AddComponent<ShootComponent>();
      shtcmp->bulletFireRate = 5.f;
      shtcmp->bulletKnockbackForce = 800;
      shtcmp->bulletSpeed = 1000.f;

      transfxcmp->bindMin = camManager->GetScreenToWorld({0, 0});
      transfxcmp->bindMax = camManager->GetScreenToWorld({rctx->gameWidth, rctx->gameHeight});
      break;
    }
  }
  else
  {
    _spawnTimer += dt;
  }
}
