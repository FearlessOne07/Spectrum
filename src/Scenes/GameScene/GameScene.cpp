#include "GameScene.hpp"
#include "base/SystemManager.hpp"
#include "raylib.h"

void GameScene::Update(float dt, Base::SystemManager *systemManager)
{
}

void GameScene::Enter( //
  Base::SystemManager *systemManager, Base::AssetManager *assetManager,
  Base::SceneData sceneData //
)
{
}

void GameScene::Render(Base::SystemManager *systemManager)
{
  DrawText(TextFormat("FPS: %i", GetFPS()), 0, 0, 40, WHITE);
  systemManager->Render();
}

void GameScene::Exit(Base::SystemManager *systemManager, Base::AssetManager *assetManager)
{
}

void GameScene::GetInput()
{
}
