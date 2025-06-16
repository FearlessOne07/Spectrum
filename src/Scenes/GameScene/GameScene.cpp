#include "GameScene.hpp"
#include "Layers/GameUILayer.hpp"
#include "Layers/MainGameLayer.hpp"
#include "Layers/ParticleLayer.hpp"
#include "Scenes/PauseMenu/PauseMenu.hpp"
#include "Systems/BulletSystem/BulletSystem.hpp"
#include "Systems/HealthSystem/HealthSystem.hpp"
#include "Systems/TrackingSystem/TrackingSystem.hpp"
#include "Systems/TransformEffectsSystem/TransformEffectsSystem.hpp"
#include "base/audio/Sound.hpp"
#include "base/audio/signals/PlayAudioStreamSignal.hpp"
#include "base/scenes/SceneTransition.hpp"
#include "base/shaders/Shader.hpp"
#include "base/signals/SignalBus.hpp"
#include <base/renderer/RenderContext.hpp>
#include <base/renderer/RenderContextSingleton.hpp>
#include <base/systems/EntityCollisionSystem.hpp>
#include <base/systems/InputSystem.hpp>
#include <base/systems/MoveSystem.hpp>
#include <base/systems/RenderSystem.hpp>
#include <base/systems/SystemManager.hpp>
#include <memory>
#include <raylib.h>

void GameScene::Enter(Base::SceneData sceneData)
{
  SetClearColor({7, 7, 15, 255});

  // Register Events
  auto bus = Base::SignalBus::GetInstance();

  // Init Camera
  const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();
  //
  Base::SystemManager *systemManager = GetSystemManager();

  // Activate Systems
  systemManager->ActivatSystem<Base::RenderSystem>();
  systemManager->ActivatSystem<Base::MoveSystem>();
  systemManager->ActivatSystem<Base::InputSystem>();
  systemManager->ActivatSystem<Base::EntityCollisionSystem>();

  systemManager->ActivatSystem<TrackingSystem>();
  systemManager->ActivatSystem<BulletSystem>();
  systemManager->ActivatSystem<HealthSystem>();
  systemManager->ActivatSystem<TransformEffectsSystem>();

  LoadAsset<Base::BaseShader>("assets/shaders/vignette.frag");
  LoadAsset<Base::Texture>("assets/textures/ship.png");
  LoadAsset<Base::Texture>("assets/textures/chaser.png");
  LoadAsset<Base::Texture>("assets/textures/shooter.png");
  LoadAsset<Base::Texture>("assets/textures/shooter-bullet.png");
  LoadAsset<Base::Texture>("assets/textures/player-bullet.png");
  LoadAsset<Base::Sound>("assets/sounds/bullet-fire.wav");
  LoadAsset<Base::Sound>("assets/sounds/enemy-die.wav");
  LoadAsset<Base::Sound>("assets/sounds/player-hit.wav");

  // auto uiLayer = AddRenderLayer({1920, 1080});
  // AttachLayer<GameUILayer>(uiLayer);

  // MainRenderLayer
  auto mainLayer = AddRenderLayer({640, 360});
  mainLayer->SetCameraOffset({640.f / 2, 360.f / 2});
  mainLayer->SetCameraZoom(0.5);
  mainLayer->SetCameraTarget({0, 0});
  mainLayer->SetCameraRotation(0);
  mainLayer->SetCameraMode(Base::Camera2DExtMode::STATIC);

  auto vignetteShader = GetAsset<Base::BaseShader>("vignette");
  mainLayer->AddShaderPass(vignetteShader);
  mainLayer->SetShaderUniform(vignetteShader, "u_resolution", Vector2{1920, 1080});

  AttachLayer<MainGameLayer>(mainLayer);
  AttachLayer<ParticleLayer>(mainLayer);

  std::shared_ptr<Base::PlayAudioStreamSignal> sig = std::make_shared<Base::PlayAudioStreamSignal>();
  sig->streamHandle = GetAsset<Base::AudioStream>("main-track");
  sig->streamPan = 0.5;
  sig->streamVolume = 0.3;
  sig->loopStream = true;
  bus->BroadCastSignal(sig);
}

void GameScene::Exit()
{
  GetSystemManager()->DeactivateActiveSystems();
}

void GameScene::OnInputEvent(std::shared_ptr<Base::InputEvent> event)
{
  if (auto keyEvent = std::dynamic_pointer_cast<Base::KeyEvent>(event))
  {
    if (keyEvent->key == KEY_ESCAPE && keyEvent->action == Base::InputEvent::Action::PRESSED)
    {
      SetSceneTransition<PauseMenu>(Base::SceneRequest::PUSH_NEW_SCENE);
      keyEvent->isHandled = true;
    }
  }
}

void GameScene::Suspend()
{
  GetSystemManager()->SuspendAllSystems();
}

void GameScene::Resume()
{
  GetSystemManager()->UnsuspendSuspendedSystems();
};
