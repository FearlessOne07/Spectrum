#include "SpawnManager.hpp"
#include "Components/GemComponent/GemComponent.hpp"
#include "Components/ShootComponent/ShootComponent.hpp"
#include "base/Entity.hpp"
#include "base/EntityManager.hpp"
#include "base/RenderContext.hpp"
#include "base/RenderContextSingleton.hpp"
#include "base/components/BoundingBoxComponent.hpp"
#include "base/components/CameraComponent.hpp"
#include "base/components/InputComponent.hpp"
#include "base/components/MoveComponent.hpp"
#include "base/components/ShapeComponent.hpp"
#include "base/components/TransformComponent.hpp"
#include "raylib/raylib.h"
#include <cstddef>

size_t SpawnManager::SpawnPlayer(Base::EntityManager *entityManager, Vector2 position)
{
  Base::Entity *e = entityManager->AddEntity();

  auto *transcmp = e->GetComponent<Base::TransformComponent>();
  transcmp->position = position;

  // Input comp registeration
  auto *mvcmp = e->AddComponent<Base::MoveComponent>();
  mvcmp->speed = 1000.f;
  mvcmp->acceleration = 3;

  auto *shtcmp = e->AddComponent<ShootComponent>();
  shtcmp->bulletFireRate = 1;
  shtcmp->bulletLifetime = 3;
  shtcmp->bulletSpeed = 2000.f;

  auto *shpcmp = e->AddComponent<Base::ShapeComponent>();
  shpcmp->fill = true;
  shpcmp->color = WHITE;
  shpcmp->points = 8;
  shpcmp->radius = 30;

  auto *abbcmp = e->AddComponent<Base::BoundingBoxComponent>();
  abbcmp->size = {.x = shpcmp->radius * 2, .y = shpcmp->radius * 2};
  abbcmp->positionOffset = {.x = shpcmp->radius, .y = shpcmp->radius};
  abbcmp->draw = true;
  abbcmp->fill = false;
  abbcmp->color = RED;
  abbcmp->nonFillThickness = 2;

  auto *inpcmp = e->AddComponent<Base::InputComponent>();
  inpcmp->BindKeyPressed(KEY_A, [mvcmp]() { mvcmp->targetVelocity.x = -1; });
  inpcmp->BindKeyPressed(KEY_D, [mvcmp]() { mvcmp->targetVelocity.x = 1; });
  inpcmp->BindKeyPressed(KEY_W, [mvcmp]() { mvcmp->targetVelocity.y = -1; });
  inpcmp->BindKeyPressed(KEY_S, [mvcmp]() { mvcmp->targetVelocity.y = 1; });

  inpcmp->BindKeyReleased(KEY_A, [mvcmp]() { mvcmp->targetVelocity.x = 0; });
  inpcmp->BindKeyReleased(KEY_D, [mvcmp]() { mvcmp->targetVelocity.x = 0; });
  inpcmp->BindKeyReleased(KEY_S, [mvcmp]() { mvcmp->targetVelocity.y = 0; });
  inpcmp->BindKeyReleased(KEY_W, [mvcmp]() { mvcmp->targetVelocity.y = 0; });

  inpcmp->BindMouseButtonDown(MOUSE_BUTTON_LEFT, [shtcmp]() {
    const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();
    shtcmp->IsFiring = true;
    shtcmp->target = GetScreenToWorld2D(rd->GetScreenToGame(GetMousePosition()), rd->camera);
  });

  auto *camcmp = e->AddComponent<Base::CameraComponent>();
  camcmp->cameraMode = Base::CameraMode::SMOOTH_FOLLOW;
  camcmp->maxFollowDistance = 200.f;
  camcmp->cameraSpeed = 1000.f;

  auto *gemcmp = e->AddComponent<GemComponent>();
  return e->GetID();
}
