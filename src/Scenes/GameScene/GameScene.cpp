#include "GameScene.hpp"
#include "Layers/GameUILayer.hpp"
#include "Layers/MainGameLayer.hpp"
#include "Layers/ParticleLayer.hpp"
#include "Scenes/GameScene/SharedGameData.hpp"
#include "ShaderEffects/Bloom/Bloom.hpp"
#include "ShaderEffects/Vignette/Vignette.hpp"
#include "Systems/BulletSystem/BulletSystem.hpp"
#include "base/assets/BaseAsset.hpp"
#include "base/audio/signals/PlayAudioStreamSignal.hpp"
#include "base/rendering/GeometryType.hpp"
#include "base/rendering/Origin.hpp"
#include "base/rendering/RenderContextSingleton.hpp"
#include "base/signals/SignalBus.hpp"
#include <memory>

void GameScene::Enter(const Base::SceneData &sceneData)
{
  Engine().Entities->Clear();
  Engine().Systems->StartSystems();
  SetClearColor({7, 7, 15, 255});

  // Init Shared Data
  InitSharedData<SharedGameData>();
  GetSharedData<SharedGameData>()->PlayerShip = sceneData.Get<Ship>();

  // Register Events
  auto bus = Base::SignalBus::GetInstance();
  const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();
  Base::Ref<Base::SystemManager> systemManager = Engine().Systems;

  // Activate Systems
  Engine().Assets->LoadTexture("assets/textures/power-ups.png");
  Engine().Assets->LoadTexture("assets/textures/heart-ui.png");
  Engine().Assets->LoadTexture("assets/textures/entities.png");
  Engine().Assets->LoadSound("assets/sounds/bullet-fire.wav");
  Engine().Assets->LoadSound("assets/sounds/enemy-die.wav");
  Engine().Assets->LoadSound("assets/sounds/player-hit.wav");
  Engine().Assets->LoadShader(Base::ShaderPath{
    "",
    "assets/shaders/vignette/vignette.frag",
    Base::GeometryType::Texture,
  });

  auto uiLayer =
    Engine().Rendering->InitLayer(shared_from_this(), {0, 0}, Base::Blank, {.Width = 1920, .Height = 1080});
  AttachLayer<GameUILayer>(uiLayer);

  // MainRenderLayer
  Base::Vector2 mainLayerRes = Base::Vector2{rd->gameWidth, rd->gameHeight} / 4.f;
  auto mainLayer = Engine().Rendering->InitLayer( //
    shared_from_this(), {0, 0}, GetClearColor(),
    {
      .Width = static_cast<int>(mainLayerRes.x),
      .Height = static_cast<int>(mainLayerRes.y),
    } //
  );
  mainLayer->SetCameraZoom(mainLayerRes.x / rd->gameWidth);
  mainLayer->SetCameraTarget({0, 0});
  mainLayer->SetCameraRotation(0);
  mainLayer->SetCamerOriginPoint(Base::Origin::Center);

  // TODO: Fix Tone mapping for bloom??
  mainLayer->AddShaderEffect<Vignette>(shared_from_this(), Base::Color{255, 48, 48, 255}, 0.5f, 1);

  AttachLayer<MainGameLayer>(mainLayer);
  AttachLayer<ParticleLayer>(mainLayer);

  auto bloom = AddPostProcessingEffect<Bloom>(1.2, 0.25, 0.25);

  std::shared_ptr<Base::PlayAudioStreamSignal> sig = std::make_shared<Base::PlayAudioStreamSignal>();
  sig->streamHandle = Engine().Assets->GetAsset<Base::AudioStream>("game-track", true);
  sig->streamPan = 0.5;
  sig->streamVolume = 0.5;
  sig->loopStream = true;
  bus->BroadCastSignal(sig);
}

void GameScene::Exit()
{
  Engine().Systems->StopSystems();
  Engine().Entities->Clear();
}

void GameScene::OnInputEvent(std::shared_ptr<Base::InputEvent> event)
{
}

void GameScene::Suspend()
{
  Engine().Systems->Suspend();
}

void GameScene::Resume()
{
  Engine().Systems->Unsuspend();
};

void GameScene::Pause()
{
}

void GameScene::UnPause()
{
}
