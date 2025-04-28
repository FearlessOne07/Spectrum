#include "GameScene.hpp"
#include "Systems/BulletSystem/BulletSystem.hpp"
#include "Systems/HealthSystem/HealthSystem.hpp"
#include "Systems/TrackingSystem/TrackingSystem.hpp"
#include "Systems/TransformEffectsSystem/TransformEffectsSystem.hpp"
#include "base/EventBus.hpp"
#include "base/input/Events/MouseButtonEvent.hpp"
#include "base/particles/ParticleEmitter.hpp"
#include "raylib/raylib.h"
#include <base/RenderContext.hpp>
#include <base/RenderContextSingleton.hpp>
#include <base/SystemManager.hpp>
#include <base/systems/EntityCollisionSystem.hpp>
#include <base/systems/InputSystem.hpp>
#include <base/systems/MoveSystem.hpp>
#include <base/systems/RenderSystem.hpp>
#include <base/ui/UIManager.hpp>
#include <memory>
#include <random>
#include <raylib.h>

void GameScene::Enter( //
  Base::SystemManager *systemManager, Base::AssetManager *assetManager,
  Base::SceneData sceneData //
)
{
  // Register Events
  auto bus = Base::EventBus::GetInstance();
  bus->SubscribeEvent<Base::MouseButtonEvent>([this](const std::shared_ptr<Base::Event> &event) {
    this->OnMouseEvent(std::static_pointer_cast<Base::MouseButtonEvent>(event));
  });
  // Init Camera
  const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();
  rd->camera.offset = {rd->gameWidth / 2, rd->gameHeight / 2};
  rd->camera.zoom = 1;
  rd->camera.target = {0, 0};
  rd->camera.rotation = 0.f;

  // Activate Systems
  systemManager->ActivatSystem<Base::RenderSystem>();
  systemManager->ActivatSystem<Base::MoveSystem>();
  systemManager->ActivatSystem<Base::InputSystem>();
  systemManager->ActivatSystem<BulletSystem>();
  systemManager->ActivatSystem<TrackingSystem>();
  systemManager->ActivatSystem<Base::EntityCollisionSystem>();
  systemManager->ActivatSystem<HealthSystem>();
  systemManager->ActivatSystem<TransformEffectsSystem>();

  // Wave manager
  _waveManager = WaveManager(GetEntityManager());
  _waveManager.SpawnPlayer(assetManager);

  // Emitter
  _emitter = GetParticleManager()->AddEmitter();
  _emitter->particleStartSize = {50, 50};
  _emitter->particleLifeTime = 0.5f;
  _emitter->position = GetMousePosition();
  _emitter->isEmitting = false;

  // Event Handler
  _playerEVH.Init();
}

void GameScene::Update(float dt, Base::SystemManager *systemManager)
{
  GetInput();
  GetUIManager()->Update();
  _waveManager.SpawnWaves(dt);
  systemManager->Update(dt);
  GetParticleManager()->Update(dt);
}

void GameScene::Render(Base::SystemManager *systemManager)
{
  DrawText(TextFormat("FPS: %i", GetFPS()), 0, 0, 40, WHITE);

  const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();
  BeginMode2D(rd->camera);
  GetParticleManager()->Render();
  systemManager->Render();
  EndMode2D();
}

void GameScene::Exit(Base::SystemManager *systemManager, Base::AssetManager *assetManager)
{
  systemManager->DeactivateActiveSystems();
}

void GameScene::GetInput()
{
}

void GameScene::OnMouseEvent(const std::shared_ptr<Base::MouseButtonEvent> &event)
{
  std::random_device rd;
  std::mt19937_64 gen(rd());

  std::uniform_real_distribution<float> angleDist(0, 360);
  const Base::RenderContext *rctx = Base::RenderContextSingleton::GetInstance();

  if (event->button == MOUSE_BUTTON_LEFT && event->action == Base::MouseButtonEvent::Action::HELD)
  {
    float angle = angleDist(gen);
    _emitter->particleDirection = {static_cast<float>(sin(angle * DEG2RAD)), static_cast<float>(cos(angle * DEG2RAD))};
    _emitter->isEmitting = true;
    _emitter->particleShape = Base::ParticleEmitter::ParticleShape::CIRCLE;
    _emitter->position = GetScreenToWorld2D(rctx->GetScreenToGame(GetMousePosition()), rctx->camera);
    _emitter->particleSpeed = std::uniform_real_distribution<float>(1000)(gen);
  }
  else if (event->button == MOUSE_LEFT_BUTTON && event->action == Base::MouseButtonEvent::Action::RELEASED)
  {
    _emitter->isEmitting = false;
  }
}
