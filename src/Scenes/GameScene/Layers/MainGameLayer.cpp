#include "MainGameLayer.hpp"
#include "base/game/RenderContext.hpp"
#include "base/scenes/Scene.hpp"
#include "base/systems/SystemManager.hpp"
#include <base/game/RenderContextSingleton.hpp>

void MainGameLayer::OnAttach()
{
  _waveManager = WaveManager(_owner->GetEntityManager());
  _waveManager.SpawnPlayer(_owner->GetAssetManager());
  _playerEVH.Init();
}

void MainGameLayer::OnDetach()
{
}

void MainGameLayer::OnInputEvent(std::shared_ptr<Base::InputEvent> &event)
{
  _owner->GetSystemManager()->OnInputEvent(event);
}

void MainGameLayer::Update(float dt)
{
  _waveManager.SpawnWaves(dt);
}

void MainGameLayer::Render()
{
  const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();
  BeginMode2D(rd->camera);
  _owner->GetSystemManager()->Render();
  EndMode2D();
}
