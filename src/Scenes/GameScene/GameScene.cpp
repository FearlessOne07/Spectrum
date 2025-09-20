#include "GameScene.hpp"
#include "Layers/GameUILayer.hpp"
#include "Layers/MainGameLayer.hpp"
#include "Layers/ParticleLayer.hpp"
#include "Scenes/GameScene/SharedGameData.hpp"
#include "ShaderEffects/Bloom/Bloom.hpp"
#include "ShaderEffects/Vignette/Vignette.hpp"
#include "Systems/BulletSystem/BulletSystem.hpp"
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
  GetEntityManager()->Clear();
  StartSystems();
  SetClearColor({7, 7, 15, 255});

  // Init Shared Data
  InitSharedData<SharedGameData>();

  // Register Events
  auto bus = Base::SignalBus::GetInstance();
  const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();
  Base::SystemManager *systemManager = GetSystemManager();

  // Activate Systems
  LoadAsset<Base::Texture>("assets/textures/power-ups.png");
  LoadAsset<Base::Texture>("assets/textures/heart-ui.png");

  LoadAsset<Base::Texture>("assets/textures/entities.png");

  LoadAsset<Base::Sound>("assets/sounds/bullet-fire.wav");
  LoadAsset<Base::Sound>("assets/sounds/enemy-die.wav");
  LoadAsset<Base::Sound>("assets/sounds/player-hit.wav");

  LoadAsset<Base::BaseShader>("assets/shaders/bloom/bright_pass.frag");
  LoadAsset<Base::BaseShader>("assets/shaders/bloom/blur_pass.frag");
  LoadAsset<Base::BaseShader>("assets/shaders/bloom/combine_pass.frag");
  LoadAsset<Base::BaseShader>("assets/shaders/vignette/vignette.frag");

  auto uiLayer = AddRenderLayer({1920, 1080});
  AttachLayer<GameUILayer>(uiLayer);

  // MainRenderLayer
  Vector2 mainLayerRes = Vector2{rd->gameWidth, rd->gameHeight} / 4;
  auto mainLayer = AddRenderLayer({mainLayerRes.x, mainLayerRes.y}, GetClearColor());
  mainLayer->SetCameraOffset({mainLayerRes.x / 2, mainLayerRes.y / 2});
  mainLayer->SetCameraZoom(mainLayerRes.x / rd->gameWidth);
  mainLayer->SetCameraTarget({0, 0});
  mainLayer->SetCameraRotation(0);

  // TODO: Fix Tone mapping??
  mainLayer->AddShaderEffect<Bloom>(1.2, 0.25, 1);
  // mainLayer->AddShaderEffect<Vignette>(Color{123, 123, 231, 255});

  AttachLayer<MainGameLayer>(mainLayer);
  AttachLayer<ParticleLayer>(mainLayer);

  std::shared_ptr<Base::PlayAudioStreamSignal> sig = std::make_shared<Base::PlayAudioStreamSignal>();
  sig->streamHandle = GetAsset<Base::AudioStream>("game-track");
  sig->streamPan = 0.5;
  sig->streamVolume = 0.5;
  sig->loopStream = true;
  bus->BroadCastSignal(sig);
}

void GameScene::Exit()
{
  StopSystems();
  GetEntityManager()->Clear();
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

void GameScene::Pause()
{
}

void GameScene::UnPause()
{
}
