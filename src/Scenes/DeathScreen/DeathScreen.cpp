#include "Scenes/DeathScreen/DeathScreen.hpp"
#include "Layers/MainLayer.hpp"
#include "base/audio/signals/StopAudioStreamSignal.hpp"
#include "base/signals/SignalBus.hpp"
#include "base/util/Colors.hpp"

void DeathScreen::Enter(const Base::SceneData &sceneData)
{
  AttachLayer<MainLayer>(Engine().Rendering->InitLayer(shared_from_this(), {0, 0}, {1920, 1080}, Base::Black));
}

void DeathScreen::Exit()
{
  auto bus = Base::SignalBus::GetInstance();
  std::shared_ptr<Base::StopAudioStreamSignal> sig = std::make_shared<Base::StopAudioStreamSignal>();
  sig->streamHandle = Engine().Assets->GetAsset<Base::AudioStream>("game-track", true);
  bus->BroadCastSignal(sig);
}
