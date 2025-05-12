#include "MainGameLayer.hpp"
#include "base/game/RenderContext.hpp"
#include "base/input/InputEvent.hpp"
#include "base/scenes/Scene.hpp"
#include "base/systems/SystemManager.hpp"
#include <base/game/RenderContextSingleton.hpp>
#include <memory>

void MainGameLayer::OnAttach()
{
  _waveManager = WaveManager(GetOwner()->GetEntityManager());
  _waveManager.SpawnPlayer(GetOwner()->GetAssetManager(), GetOwner()->GetCameraManager());
  _playerEVH.Init(GetOwner());
}

void MainGameLayer::OnDetach()
{
}

void MainGameLayer::OnInputEvent(std::shared_ptr<Base::InputEvent> &event)
{
  GetOwner()->GetSystemManager()->OnInputEvent(event);
}

void MainGameLayer::Update(float dt)
{
  _waveManager.SpawnWaves(dt, GetOwner()->GetCameraManager());
}

void MainGameLayer::Render()
{
  const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();
  auto camManager = GetOwner()->GetCameraManager();
  camManager->BeginCameraMode();
  GetOwner()->GetSystemManager()->Render();
  camManager->EndCameraMode();
}
