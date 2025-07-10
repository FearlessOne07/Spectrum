#include "MainMenu.hpp"
#include "Layers/MainMenuLayer.hpp"
#include "base/audio/signals/PlayAudioStreamSignal.hpp"
#include "base/audio/signals/StopAudioStreamSignal.hpp"
#include "base/signals/SignalBus.hpp"

void MainMenu::Enter(Base::SceneData sceneData)
{
  AttachLayer<MainMenuLayer>(AddRenderLayer({1920, 1080}));

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
