#include "GameScene.hpp"
#include "Systems/BulletSystem/BulletSystem.hpp"
#include "Systems/HealthSystem/HealthSystem.hpp"
#include "Systems/TrackingSystem/TrackingSystem.hpp"
#include "base/RenderContext.hpp"
#include "base/RenderContextSingleton.hpp"
#include "base/SystemManager.hpp"
#include "base/systems/EntityCollisionSystem.hpp"
#include "base/systems/InputSystem.hpp"
#include "base/systems/MoveSystem.hpp"
#include "base/systems/RenderSystem.hpp"
#include "base/ui/UIManager.hpp"
#include "base/ui/elements/UIButton.hpp"
#include "raylib.h"
#include <iostream>
#include <memory>

void GameScene::Enter( //
  Base::SystemManager *systemManager, Base::AssetManager *assetManager,
  Base::SceneData sceneData //
)
{
  // Activate Systems
  systemManager->ActivatSystem<Base::RenderSystem>();
  systemManager->ActivatSystem<Base::MoveSystem>();
  systemManager->ActivatSystem<Base::InputSystem>();
  systemManager->ActivatSystem<BulletSystem>();
  systemManager->ActivatSystem<TrackingSystem>();
  systemManager->ActivatSystem<Base::EntityCollisionSystem>();
  systemManager->ActivatSystem<HealthSystem>();

  // Button
  auto button = std::make_shared<Base::UIButton>();
  button->position = {500, 500};
  button->onClick = []() { std::cout << "Noice\n"; };

  // Wave manager
  _waveManager = WaveManager(GetEntityManager());

  // Spawn Player
  _playerID = _waveManager.SpawnPlayer(assetManager);

  // Init Camera
  const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();
  rd->camera.offset = {rd->gameWidth / 2, rd->gameHeight / 2};
  rd->camera.zoom = 0.8;
  rd->camera.target = {0, 0};
  rd->camera.rotation = 0.f;

  // Event Handler
  _playerEVH.Init();
}

void GameScene::Update(float dt, Base::SystemManager *systemManager)
{
  GetInput();
  GetUIManager()->Update();
  _waveManager.SpawnWaves(dt);
  systemManager->Update(dt);
}

void GameScene::Render(Base::SystemManager *systemManager)
{
  DrawText(TextFormat("FPS: %i", GetFPS()), 0, 0, 40, WHITE);

  const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();
  BeginMode2D(rd->camera);
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
