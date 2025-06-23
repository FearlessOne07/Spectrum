#include "GameScene.hpp"
#include "Layers/GameUILayer.hpp"
#include "Layers/MainGameLayer.hpp"
#include "Layers/ParticleLayer.hpp"
#include "Systems/BulletSystem/BulletSystem.hpp"
#include "Systems/PowerUpSystem/PowerUpSystem.hpp"
#include "base/audio/Sound.hpp"
#include "base/audio/signals/PlayAudioStreamSignal.hpp"
#include "base/shaders/Shader.hpp"
#include "base/signals/SignalBus.hpp"
#include <base/renderer/RenderContext.hpp>
#include <base/renderer/RenderContextSingleton.hpp>
#include <memory>
#include <raylib.h>

void GameScene::Enter(Base::SceneData sceneData)
{
  StartSystems();
  SetClearColor({7, 7, 15, 255});

  // Register Events
  auto bus = Base::SignalBus::GetInstance();
  const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();
  Base::SystemManager *systemManager = GetSystemManager();

  // Activate Systems
  LoadAsset<Base::BaseShader>("assets/shaders/vignette.frag");
  LoadAsset<Base::Texture>("assets/textures/ship.png");
  LoadAsset<Base::Texture>("assets/textures/power-ups.png");
  LoadAsset<Base::Texture>("assets/textures/heart-ui.png");
  LoadAsset<Base::Texture>("assets/textures/king.png");
  LoadAsset<Base::Texture>("assets/textures/chaser.png");
  LoadAsset<Base::Texture>("assets/textures/shooter.png");
  LoadAsset<Base::Texture>("assets/textures/shooter-bullet.png");
  LoadAsset<Base::Texture>("assets/textures/player-bullet.png");
  LoadAsset<Base::Sound>("assets/sounds/bullet-fire.wav");
  LoadAsset<Base::Sound>("assets/sounds/enemy-die.wav");
  LoadAsset<Base::Sound>("assets/sounds/player-hit.wav");

  auto uiLayer = AddRenderLayer({1920, 1080});
  AttachLayer<GameUILayer>(uiLayer);

  // MainRenderLayer
  auto mainLayer = AddRenderLayer({640, 360}, GetClearColor());
  mainLayer->SetCameraOffset({640.f / 2, 360.f / 2});
  mainLayer->SetCameraZoom(0.35);
  mainLayer->SetCameraTarget({0, 0});
  mainLayer->SetCameraRotation(0);
  mainLayer->SetCameraMode(Base::Camera2DExtMode::SMOOTH_FOLLOW);

  AttachLayer<MainGameLayer>(mainLayer);
  AttachLayer<ParticleLayer>(mainLayer);

  std::shared_ptr<Base::PlayAudioStreamSignal> sig = std::make_shared<Base::PlayAudioStreamSignal>();
  sig->streamHandle = GetAsset<Base::AudioStream>("main-track");
  sig->streamPan = 0.5;
  sig->streamVolume = 1;
  sig->loopStream = true;
  bus->BroadCastSignal(sig);
}

void GameScene::Exit()
{
  StopSystems();
}

void GameScene::OnInputEvent(std::shared_ptr<Base::InputEvent> event)
{
}

void GameScene::Suspend()
{
  SuspendSystems();
}

void GameScene::Resume()
{
  UnsuspendSystems();
};

void GameScene::InitPauseMenu()
{
}

void GameScene::Pause()
{
}

void GameScene::UnPause()
{
}
