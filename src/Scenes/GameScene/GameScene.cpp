#include "GameScene.hpp"
#include "Layers/MainGameLayer.hpp"
#include "Layers/ParticleLayer.hpp"
#include "Systems/BulletSystem/BulletSystem.hpp"
#include "Systems/HealthSystem/HealthSystem.hpp"
#include "Systems/TrackingSystem/TrackingSystem.hpp"
#include "Systems/TransformEffectsSystem/TransformEffectsSystem.hpp"
#include "base/signals/SignalManager.hpp"
#include <base/game/RenderContext.hpp>
#include <base/game/RenderContextSingleton.hpp>
#include <base/systems/EntityCollisionSystem.hpp>
#include <base/systems/InputSystem.hpp>
#include <base/systems/MoveSystem.hpp>
#include <base/systems/RenderSystem.hpp>
#include <base/systems/SystemManager.hpp>
#include <raylib.h>

void GameScene::Enter(Base::SceneData sceneData)
{
  // Register Events
  auto bus = Base::SignalManager::GetInstance();

  // Init Camera
  const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();
  rd->camera.offset = {rd->gameWidth / 2, rd->gameHeight / 2};
  rd->camera.zoom = 1;
  rd->camera.target = {0, 0};
  rd->camera.rotation = 0.f;

  Base::SystemManager *systemManager = GetSystemManager();

  // Activate Systems
  systemManager->ActivatSystem<Base::RenderSystem>();
  systemManager->ActivatSystem<Base::MoveSystem>();
  systemManager->ActivatSystem<Base::InputSystem>();
  systemManager->ActivatSystem<BulletSystem>();
  systemManager->ActivatSystem<TrackingSystem>();
  systemManager->ActivatSystem<Base::EntityCollisionSystem>();
  systemManager->ActivatSystem<HealthSystem>();
  systemManager->ActivatSystem<TransformEffectsSystem>();

  // Layers
  GetLayerStack().AttachLayer<MainGameLayer>();
  GetLayerStack().AttachLayer<ParticleLayer>();

  // Event Handler
}

void GameScene::Update(float dt)
{
  GetLayerStack().Update(dt);
}

void GameScene::Render()
{
  DrawText(TextFormat("FPS: %i", GetFPS()), 0, 0, 40, WHITE);
  GetLayerStack().Render();
}

void GameScene::Exit()
{
  GetSystemManager()->DeactivateActiveSystems();
}
