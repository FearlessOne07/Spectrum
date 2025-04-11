#include "SpawnManager.hpp"
#include "Components/DamageComponent.hpp"
#include "Components/HealthComponent.hpp"
#include "Components/ShootComponent.hpp"
#include "Components/Tags/EnemyTag.hpp"
#include "Components/Tags/PlayerTag.hpp"
#include "Components/TrackingComponent.hpp"
#include "base/Entity.hpp"
#include "base/EntityManager.hpp"
#include "base/RenderContext.hpp"
#include "base/RenderContextSingleton.hpp"
#include "base/components/BoundingBoxComponent.hpp"
#include "base/components/ImpulseComponent.hpp"
#include "base/components/InputComponent.hpp"
#include "base/components/MoveComponent.hpp"
#include "base/components/RigidBodyComponent.hpp"
#include "base/components/ShapeComponent.hpp"
#include "base/components/TransformComponent.hpp"
#include "raylib.h"
#include "raylib/raylib.h"
#include <cstddef>
#include <random>

size_t SpawnManager::SpawnPlayer(Base::EntityManager *entityManager, Vector2 position)
{
  Base::Entity *e = entityManager->AddEntity();

  auto *transcmp = e->GetComponent<Base::TransformComponent>();
  transcmp->position = position;

  // Input comp registeration
  auto *mvcmp = e->AddComponent<Base::MoveComponent>();
  mvcmp->driveForce = 5000;
  mvcmp->brakeForce = 500.f;

  auto rbcmp = e->AddComponent<Base::RigidBodyComponent>();
  rbcmp->isKinematic = false;
  rbcmp->mass = 1;
  rbcmp->drag = 3;

  auto *shtcmp = e->AddComponent<ShootComponent>();
  shtcmp->bulletFireRate = 0.5;
  shtcmp->bulletLifetime = 3;
  shtcmp->bulletForce = 2000.f;

  auto *shpcmp = e->AddComponent<Base::ShapeComponent>();
  shpcmp->fill = true;
  shpcmp->color = WHITE;
  shpcmp->points = 8;
  shpcmp->radius = 30;

  auto *abbcmp = e->AddComponent<Base::BoundingBoxComponent>();
  abbcmp->size = {.x = shpcmp->radius * 2, .y = shpcmp->radius * 2};
  abbcmp->positionOffset = {.x = shpcmp->radius, .y = shpcmp->radius};
  abbcmp->SetTypeFlag(Base::BoundingBoxComponent::Type::HURTBOX);
  abbcmp->draw = true;
  abbcmp->fill = false;
  abbcmp->color = RED;

  auto *inpcmp = e->AddComponent<Base::InputComponent>();
  inpcmp->BindKeyPressed(KEY_A, [rbcmp]() { rbcmp->direction.x = -1; });
  inpcmp->BindKeyPressed(KEY_D, [rbcmp]() { rbcmp->direction.x = 1; });
  inpcmp->BindKeyPressed(KEY_W, [rbcmp]() { rbcmp->direction.y = -1; });
  inpcmp->BindKeyPressed(KEY_S, [rbcmp]() { rbcmp->direction.y = 1; });

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

  return e->GetID();
}

void SpawnManager::SpawnEnemies(float dt, Base::EntityManager *entityManager, size_t playerID)
{

  if (_spawnTimer >= _spawnDuration)
  {
    _spawnTimer = 0.f;
    const Base::RenderContext *rctx = Base::RenderContextSingleton::GetInstance();

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

    auto *transcmp = e->GetComponent<Base::TransformComponent>();
    transcmp->position = position;

    auto *trckcmp = e->AddComponent<TrackingComponent>();
    trckcmp->targetEntityID = playerID;

    auto *mvcmp = e->AddComponent<Base::MoveComponent>();
    mvcmp->driveForce = std::uniform_int_distribution(1000, 1000)(gen);

    auto *rbcmp = e->AddComponent<Base::RigidBodyComponent>();
    rbcmp->isKinematic = false;
    rbcmp->drag = 3;
    rbcmp->mass = 1;

    auto *shpcmp = e->AddComponent<Base::ShapeComponent>();
    shpcmp->color = RED;
    shpcmp->fill = true;
    shpcmp->fillOutline = true;
    shpcmp->outlineColor = WHITE;
    shpcmp->points = 6;
    shpcmp->radius = 30;

    auto *abbcmp = e->AddComponent<Base::BoundingBoxComponent>();
    abbcmp->size = {shpcmp->radius * 2, shpcmp->radius * 2};
    abbcmp->positionOffset = {shpcmp->radius, shpcmp->radius};
    abbcmp->SetTypeFlag(Base::BoundingBoxComponent::Type::HURTBOX);
    abbcmp->SetTypeFlag(Base::BoundingBoxComponent::Type::HITBOX);

    auto hlthcmp = e->AddComponent<HealthComponent>();
    hlthcmp->health = 8;

    auto dmgcmp = e->AddComponent<DamageComponent>();
    dmgcmp->damage = 2;

    e->AddComponent<EnemyTag>();
  }
  else
  {
    _spawnTimer += dt;
  }
}
