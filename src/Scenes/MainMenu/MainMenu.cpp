#include "MainMenu.hpp"
#include "Layers/MainMenuLayer.hpp"
#include "Scenes/MainMenu/Layers/ShipSelectionLayer.hpp"
#include "ShaderEffects/Bloom/Bloom.hpp"
#include "base/audio/signals/PlayAudioStreamSignal.hpp"
#include "base/audio/signals/StopAudioStreamSignal.hpp"
#include "base/renderer/RenderContextSingleton.hpp"
#include "base/signals/SignalBus.hpp"

void MainMenu::Enter(const Base::SceneData &sceneData)
{

  // MainRenderLayer
  const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();
  Vector2 mainLayerRes = Vector2{rd->gameWidth, rd->gameHeight};
  auto mainLayer =
    GameCtx().Rendering->InitLayer(shared_from_this(), {0, 0}, {mainLayerRes.x, mainLayerRes.y}, GetClearColor());
  mainLayer->AddShaderEffect<Bloom>(shared_from_this(), 1.2, 0.25, 0.25);

  AttachLayer<MainMenuLayer>(mainLayer);
  AttachLayer<ShipSelectionLayer>(mainLayer);

  auto bus = Base::SignalBus::GetInstance();
  std::shared_ptr<Base::PlayAudioStreamSignal> sig = std::make_shared<Base::PlayAudioStreamSignal>();
  sig->streamHandle = GameCtx().Assets->GetGlobalAsset<Base::AudioStream>("main-menu-track");
  sig->streamPan = 0.5;
  sig->streamVolume = 1;
  sig->loopStream = true;
  bus->BroadCastSignal(sig);
}

void MainMenu::Exit()
{
  auto bus = Base::SignalBus::GetInstance();
  std::shared_ptr<Base::StopAudioStreamSignal> sig = std::make_shared<Base::StopAudioStreamSignal>();
  sig->streamHandle = GameCtx().Assets->GetGlobalAsset<Base::AudioStream>("main-menu-track");
  bus->BroadCastSignal(sig);
}
