#include "Spawner.hpp"
#include "Components/DamageComponent.hpp"
#include "Components/EnemyComponent.hpp"
#include "Components/HealthComponent.hpp"
#include "Components/LightCollectorComponent.hpp"
#include "Components/PowerUpComponent.hpp"
#include "Components/ShootComponent.hpp"
#include "Components/Tags/PlayerTag.hpp"
#include "Components/TrackingComponent.hpp"
#include "Components/TransformEffects.hpp"
#include "Signals/PlayerSpawnedSignal.hpp"
#include "base/components/AnimationComponent.hpp"
#include "base/components/SpriteComponent.hpp"
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
#include <base/renderer/RenderContext.hpp>
#include <base/renderer/RenderContextSingleton.hpp>
#include <base/scenes/SceneLayer.inl>
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

Spawner::Spawner(const Base::SceneLayer *parentLayer) : _parentLayer(parentLayer)
{
}

int Spawner::GetToSpawnCount() const
{
  return _toSpawn.size();
}

Base::EntityID Spawner::SpawnPlayer( //
  Base::EntityManager *entityManager,
  Vector2 position //
)
{
  auto e = entityManager->CreateEntity();
  const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();

  auto *transcmp = e->GetComponent<Base::TransformComponent>();
  transcmp->position = position;

  auto *transfxcmp = e->AddComponent<TransformEffectsComponent>();
  transfxcmp->targetAngularVelocity = 180;
  transfxcmp->angularAcceleration = 1;
  transfxcmp->bind = true;
  transfxcmp->bindMin = _parentLayer->GetScreenToWorld({0, 0});
  transfxcmp->bindMax = _parentLayer->GetScreenToWorld(_parentLayer->GetSize());
  transfxcmp->rotate = true;

  auto *mvcmp = e->AddComponent<Base::MoveComponent>();
  mvcmp->driveForce = 1500;
  mvcmp->brakeForce = 500.f;

  auto rbcmp = e->AddComponent<Base::RigidBodyComponent>();
  rbcmp->isKinematic = false;
  rbcmp->mass = 1;
  rbcmp->drag = 3;

  auto hlthcmp = e->AddComponent<HealthComponent>(20.f);

  auto *shtcmp = e->AddComponent<ShootComponent>();
  shtcmp->bulletFireRate = 0.6;
  shtcmp->bulletLifetime = 3;
  shtcmp->bulletFireTimer = 1;
  shtcmp->bulletSpeed = 1500.f;

  auto sprtcmp = e->AddComponent<Base::SpriteComponent>(                                               //
    _parentLayer->GetAsset<Base::Texture>("ship-sheet"), Vector2{0, 0}, Vector2{8, 8}, Vector2{64, 64} //
  );
  shtcmp->bulletTexture = _parentLayer->GetAsset<Base::Texture>("player-bullet");

  auto animcmp = e->AddComponent<Base::AnimationComponent>("idle");
  animcmp->AddAnimation( //
    "idle",              //
    {
      {{0, 0}, {8, 8}, 0.15},
      {{8, 0}, {8, 8}, 0.15},
      {{16, 0}, {8, 8}, 0.15},
      {{24, 0}, {8, 8}, 0.15},
    } //
  );

  auto *abbcmp = e->AddComponent<Base::ColliderComponent>();
  abbcmp->radius = sprtcmp->GetTargetSize().x / 2;
  abbcmp->shape = Base::ColliderComponent::Shape::CIRCLE;
  abbcmp->SetTypeFlag(Base::ColliderComponent::Type::HURTBOX);

  auto pucmp = e->AddComponent<PowerUpComponent>();

  auto *inpcmp = e->AddComponent<Base::InputComponent>();
  inpcmp->BindKeyDown(KEY_A, [rbcmp]() { rbcmp->direction.x = -1; });
  inpcmp->BindKeyDown(KEY_D, [rbcmp]() { rbcmp->direction.x = 1; });
  inpcmp->BindKeyDown(KEY_W, [rbcmp]() { rbcmp->direction.y = -1; });
  inpcmp->BindKeyDown(KEY_S, [rbcmp]() { rbcmp->direction.y = 1; });
  inpcmp->BindKeyPressed(KEY_K, [hlthcmp]() { hlthcmp->TakeDamage(hlthcmp->GetHealth()); });

  inpcmp->BindKeyReleased(KEY_A, [rbcmp]() { rbcmp->direction.x = 0; });
  inpcmp->BindKeyReleased(KEY_D, [rbcmp]() { rbcmp->direction.x = 0; });
  inpcmp->BindKeyReleased(KEY_S, [rbcmp]() { rbcmp->direction.y = 0; });
  inpcmp->BindKeyReleased(KEY_W, [rbcmp]() { rbcmp->direction.y = 0; });

  inpcmp->BindMouseButtonDown(MOUSE_BUTTON_LEFT, [this, shtcmp]() {
    const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();
    shtcmp->IsFiring = true;
    shtcmp->target = _parentLayer->GetScreenToWorld(_parentLayer->GetLayerMousePosition());
  });
  inpcmp->BindMouseButtonReleased(MOUSE_BUTTON_LEFT, [shtcmp]() { shtcmp->IsFiring = false; });

  auto dmgcmp = e->AddComponent<DamageComponent>();
  dmgcmp->damage = 2;
  e->AddComponent<Base::ImpulseComponent>();
  e->AddComponent<PlayerTag>();
  auto lightColCmp = e->AddComponent<LightCollectorComponent>();
  lightColCmp->collectionRaius = (abbcmp->radius + (abbcmp->radius * 0.5)) / _parentLayer->GetCameraZoom();
  entityManager->AddEntity(e);

  auto bus = Base::SignalBus::GetInstance();
  std::shared_ptr<PlayerSpawnedSignal> sig = std::make_shared<PlayerSpawnedSignal>();
  sig->player = e;
  bus->BroadCastSignal(sig);
  _playerID = e->GetID();
  return _playerID;
}

void Spawner::SpawnWave( //
  float dt, Base::EntityManager *entityManager,
  Base::EntityID playerID //
)
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
    Vector2 max = {_parentLayer->GetSize().x + _spawnOffset, _parentLayer->GetSize().y + _spawnOffset};

    Vector2 position = {0, 0};
    float ypos = 0.f;
    float xpos = 0.f;

    switch (side)
    {
    case 1: // Right
      ypos = std::uniform_real_distribution<float>(min.y, max.y)(gen);
      position = _parentLayer->GetScreenToWorld({max.x, ypos});
      break;
    case 2: // Left
      ypos = std::uniform_real_distribution<float>(min.y, max.y)(gen);
      position = _parentLayer->GetScreenToWorld({min.x, ypos});
      break;
    case 3: // Up
      xpos = std::uniform_real_distribution<float>(min.x, max.x)(gen);
      position = _parentLayer->GetScreenToWorld({xpos, min.y});
      break;
    case 4: // Down
      xpos = std::uniform_real_distribution<float>(min.x, max.x)(gen);
      position = _parentLayer->GetScreenToWorld({xpos, max.y});
      break;
    }

    auto e = entityManager->CreateEntity();
    auto enemcmp = e->AddComponent<EnemyComponent>();
    enemcmp->type = type;
    auto *transcmp = e->GetComponent<Base::TransformComponent>();
    transcmp->position = position;

    e->AddComponent<Base::ImpulseComponent>();

    auto *trckcmp = e->AddComponent<TrackingComponent>();
    trckcmp->targetEntityID = playerID;

    auto *mvcmp = e->AddComponent<Base::MoveComponent>();
    mvcmp->driveForce = std::uniform_int_distribution(300, 500)(gen);

    auto *rbcmp = e->AddComponent<Base::RigidBodyComponent>();
    rbcmp->isKinematic = false;
    rbcmp->drag = 3;
    rbcmp->mass = 1;

    auto *abbcmp = e->AddComponent<Base::ColliderComponent>();
    abbcmp->shape = Base::ColliderComponent::Shape::CIRCLE;
    abbcmp->SetTypeFlag(Base::ColliderComponent::Type::HURTBOX);
    abbcmp->SetTypeFlag(Base::ColliderComponent::Type::HITBOX);

    auto hlthcmp = e->AddComponent<HealthComponent>(8.f);

    auto dmgcmp = e->AddComponent<DamageComponent>();
    dmgcmp->damage = 1;

    auto transfxcmp = e->AddComponent<TransformEffectsComponent>();
    transfxcmp->rotate = true;
    transfxcmp->angularAcceleration = 0.5;
    transfxcmp->targetAngularVelocity = 90;
    transfxcmp->bind = false;
    transfxcmp->lookAt = true;
    transfxcmp->lookAtTarget = _playerID;

    // Sprite
    Base::SpriteComponent *sprtcmp = nullptr;

    switch (type)
    {
    case EnemyType::CHASER: {
      sprtcmp = e->AddComponent<Base::SpriteComponent>(                                                //
        _parentLayer->GetAsset<Base::Texture>("chaser"), Vector2{0, 0}, Vector2{8, 8}, Vector2{64, 64} //
      );
      enemcmp->value = 5;
      break;
    }
    case EnemyType::SHOOTER: {
      sprtcmp = e->AddComponent<Base::SpriteComponent>(                                                 //
        _parentLayer->GetAsset<Base::Texture>("shooter"), Vector2{0, 0}, Vector2{8, 8}, Vector2{64, 64} //
      );
      trckcmp->trackingDistance = 1000;
      enemcmp->value = 2;

      auto *shtcmp = e->AddComponent<ShootComponent>();
      shtcmp->bulletFireRate = 5.f;
      shtcmp->bulletKnockbackForce = 800;
      shtcmp->bulletSpeed = 1000.f;
      shtcmp->bulletTexture = _parentLayer->GetAsset<Base::Texture>("shooter-bullet");

      transfxcmp->bindMin = _parentLayer->GetScreenToWorld({0, 0});
      transfxcmp->bindMax = _parentLayer->GetScreenToWorld(_parentLayer->GetSize());
      break;
    }
    case EnemyType::KAMAKAZI:
      break;
    case EnemyType::NONE:
      break;
    }

    abbcmp->radius = sprtcmp->GetTargetSize().x / 2;
    entityManager->AddEntity(e);
  }
  else
  {
    _spawnTimer += dt;
  }
}
