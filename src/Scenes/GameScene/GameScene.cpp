#include "GameScene.hpp"
#include "Assets/GlobalAssets.hpp"
#include "Layers/GameUILayer.hpp"
#include "Layers/MainGameLayer.hpp"
#include "Layers/ParticleLayer.hpp"
#include "Scenes/GameScene/GameSceneAssets.hpp"
#include "Scenes/GameScene/SharedGameData.hpp"
#include "ShaderEffects/Bloom/Bloom.hpp"
#include "ShaderEffects/Vignette/Vignette.hpp"
#include "Ship/ShipDataBase.hpp"
#include "Systems/BulletSystem/BulletSystem.hpp"
#include "base/audio/signals/PlayAudioStreamSignal.hpp"
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
  SharedData<SharedGameData>()->PlayerShip = sceneData.Get<ShipName>();

  // Register Events
  auto bus = Base::SignalBus::GetInstance();
  const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();
  Base::Ref<Base::SystemManager> systemManager = Engine().Systems;

  // Init Assets
  InitAssetStore<GameSceneAssets>();

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
  mainLayer->AddShaderEffect<Vignette>( //
    shared_from_this(), Base::Color{255, 48, 48, 255}, 0.5f, 1,
    AssetStore<GameSceneAssets>()->Vignette //
  );

  AttachLayer<MainGameLayer>(mainLayer);
  AttachLayer<ParticleLayer>(mainLayer);

  auto &blur = Engine().Assets->GlobalAssetStore<GlobalAssets>()->BlurPass;
  auto &bright = Engine().Assets->GlobalAssetStore<GlobalAssets>()->BrightPass;
  auto &combine = Engine().Assets->GlobalAssetStore<GlobalAssets>()->CombinePass;
  auto bloom = AddPostProcessingEffect<Bloom>(0.8, 0.05, 0.25, blur, bright, combine);

  std::shared_ptr<Base::PlayAudioStreamSignal> sig = std::make_shared<Base::PlayAudioStreamSignal>();
  sig->streamHandle = Engine().Assets->GlobalAssetStore<GlobalAssets>()->GameTrack;
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
