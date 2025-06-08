#include "MainGameLayer.hpp"
#include "base/input/InputEvent.hpp"
#include "base/scenes/Scene.hpp"
#include "base/systems/SystemManager.hpp"
#include <base/renderer/RenderContext.hpp>
#include <base/renderer/RenderContextSingleton.hpp>
#include <memory>

void MainGameLayer::OnAttach()
{
  _waveManager = WaveManager(GetOwner()->GetEntityManager());
  _waveManager.SpawnPlayer(GetOwner()->GetAssetManager(), GetOwner()->GetCameraManager());
  _playerEVH.Init(GetOwner());

  GetRenderLayer()->SetShaderChain({"pixalization"});
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
  _waveManager.SpawnWaves(dt, GetOwner()->GetCameraManager(), GetOwner()->GetAssetManager());
}

void MainGameLayer::Render()
{
  const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();
  auto camManager = GetOwner()->GetCameraManager();
  camManager->BeginCameraMode();
  GetOwner()->GetSystemManager()->Render();
  camManager->EndCameraMode();
}
