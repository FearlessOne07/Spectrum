#include "GameScene.hpp"
#include "Systems/BulletSystem/BulletSystem.hpp"
#include "Systems/TrackingSystem/TrackingSystem.hpp"
#include "base/RenderContext.hpp"
#include "base/RenderContextSingleton.hpp"
#include "base/SystemManager.hpp"
#include "base/systems/EntityCollisionSystem.hpp"
#include "base/systems/InputSystem.hpp"
#include "base/systems/MoveSystem.hpp"
#include "base/systems/RenderSystem.hpp"
#include "raylib.h"

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

  // Spawn Player
  _playerID = _spawnMan.SpawnPlayer(GetEntityManager(), {0, 0});

  // Init Camera
  const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();
  rd->camera.offset = {rd->gameWidth / 2, rd->gameHeight / 2};
  rd->camera.zoom = 0.8;
  rd->camera.target = {0, 0};
}

void GameScene::Update(float dt, Base::SystemManager *systemManager)
{
  GetInput();

  _spawnMan.SpawnEnemies(dt, GetEntityManager(), _playerID);
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
