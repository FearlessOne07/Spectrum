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
  AttachLayer<MainMenuLayer>(AddRenderLayer({1920, 1080}));

  // MainRenderLayer
  const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();
  Vector2 mainLayerRes = Vector2{rd->gameWidth, rd->gameHeight} / 4;
  auto mainLayer = AddRenderLayer({mainLayerRes.x, mainLayerRes.y}, GetClearColor());
  // mainLayer->AddShaderEffect<Bloom>(1.2, 0.25, 1);

  AttachLayer<ShipSelectionLayer>(mainLayer);

  auto bus = Base::SignalBus::GetInstance();
  std::shared_ptr<Base::PlayAudioStreamSignal> sig = std::make_shared<Base::PlayAudioStreamSignal>();
  sig->streamHandle = GetAsset<Base::AudioStream>("main-menu-track");
  sig->streamPan = 0.5;
  sig->streamVolume = 1;
  sig->loopStream = true;
  bus->BroadCastSignal(sig);
}

void MainMenu::Exit()
{
  auto bus = Base::SignalBus::GetInstance();
  std::shared_ptr<Base::StopAudioStreamSignal> sig = std::make_shared<Base::StopAudioStreamSignal>();
  sig->streamHandle = GetAsset<Base::AudioStream>("main-menu-track");
  bus->BroadCastSignal(sig);
}
