#include "MainMenu.hpp"
#include "Layers/MainMenuLayer.hpp"
#include "Scenes/MainMenu/Layers/ShipSelectionLayer.hpp"
#include "ShaderEffects/Bloom/Bloom.hpp"
#include "base/audio/signals/PlayAudioStreamSignal.hpp"
#include "base/audio/signals/StopAudioStreamSignal.hpp"
#include "base/rendering/RenderContextSingleton.hpp"
#include "base/signals/SignalBus.hpp"

void MainMenu::Enter(const Base::SceneData &sceneData)
{
  // MainRenderLayer
  const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();
  Base::Vector2 mainLayerRes = Base::Vector2{rd->gameWidth, rd->gameHeight};
  auto mainLayer = Engine().Rendering->InitLayer( //
    shared_from_this(), {0, 0}, GetClearColor(),
    {
      .Width = static_cast<int>(mainLayerRes.x),
      .Height = static_cast<int>(mainLayerRes.y),
    } //
  );
  mainLayer->AddShaderEffect<Bloom>(shared_from_this(), 0.8, 0.05, 0.25);

  AttachLayer<MainMenuLayer>(mainLayer);
  AttachLayer<ShipSelectionLayer>(mainLayer);

  auto bus = Base::SignalBus::GetInstance();
  std::shared_ptr<Base::PlayAudioStreamSignal> sig = std::make_shared<Base::PlayAudioStreamSignal>();
  sig->streamHandle = Engine().Assets->GetAsset<Base::AudioStream>("main-menu-track", true);
  sig->streamPan = 0.5;
  sig->streamVolume = 1;
  sig->loopStream = true;
  bus->BroadCastSignal(sig);
}

void MainMenu::Exit()
{
  auto bus = Base::SignalBus::GetInstance();
  std::shared_ptr<Base::StopAudioStreamSignal> sig = std::make_shared<Base::StopAudioStreamSignal>();
  sig->streamHandle = Engine().Assets->GetAsset<Base::AudioStream>("main-menu-track", true);
  bus->BroadCastSignal(sig);
}
