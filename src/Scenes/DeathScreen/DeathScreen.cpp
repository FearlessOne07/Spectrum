#include "Scenes/DeathScreen/DeathScreen.hpp"
#include "Layers/MainLayer.hpp"
#include "base/audio/signals/StopAudioStreamSignal.hpp"
#include "base/signals/SignalBus.hpp"

void DeathScreen::Enter(const Base::SceneData& sceneData)
{
  AttachLayer<MainLayer>(AddRenderLayer({1920, 1080}));
}

void DeathScreen::Exit()
{
  auto bus = Base::SignalBus::GetInstance();
  std::shared_ptr<Base::StopAudioStreamSignal> sig = std::make_shared<Base::StopAudioStreamSignal>();
  sig->streamHandle = GetAsset<Base::AudioStream>("game-track");
  bus->BroadCastSignal(sig);
}
