#include "MainGameLayer.hpp"
#include "base/game/RenderContext.hpp"
#include "base/input/Events/KeyEvent.hpp"
#include "base/input/InputEvent.hpp"
#include "base/scenes/Scene.hpp"
#include "base/systems/SystemManager.hpp"
#include <base/game/RenderContextSingleton.hpp>
#include <iostream>
#include <memory>

void MainGameLayer::OnAttach()
{
  _waveManager = WaveManager(_owner->GetEntityManager());
  _waveManager.SpawnPlayer(_owner->GetAssetManager(), _owner->GetCameraManager());
  _playerEVH.Init();
}

void MainGameLayer::OnDetach()
{
}

void MainGameLayer::OnInputEvent(std::shared_ptr<Base::InputEvent> &event)
{
  if (auto keyEvent = std::dynamic_pointer_cast<Base::KeyEvent>(event))
  {
    if (keyEvent->key == KEY_T && keyEvent->action == Base::InputEvent::Action::PRESSED)
    {
      Base::CameraManager::CameraShakeConfig config;
      config.trauma = 1.f;
      config.frequency = 30.0f;
      config.shakeMagnitude = 80.0f;
      config.duration = 1.5;
      config.traumaMultiplyer = 1.5;
      config.rotationMagnitude = 1.4;

      _owner->GetCameraManager()->Shake(config);
      keyEvent->isHandled = true;
    }
  }
  _owner->GetSystemManager()->OnInputEvent(event);
}

void MainGameLayer::Update(float dt)
{
  // _waveManager.SpawnWaves(dt, _owner->GetCameraManager());
}

void MainGameLayer::Render()
{
  const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();
  auto camManager = _owner->GetCameraManager();
  camManager->BeginCameraMode();
  _owner->GetSystemManager()->Render();
  camManager->EndCameraMode();
}
