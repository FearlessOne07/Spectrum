#include "Scenes/DeathScreen/DeathScreen.hpp"
#include "Layers/MainLayer.hpp"
#include "ShaderEffects/Bloom/Bloom.hpp"
#include "SharedDeathData.hpp"
#include "base/audio/signals/StopAudioStreamSignal.hpp"
#include "base/signals/SignalBus.hpp"
#include "base/util/Colors.hpp"

void DeathScreen::Enter(const Base::SceneData &sceneData)
{

  InitSharedData<SharedDeathData>();
  SharedData<SharedDeathData>()->PlayerShip = sceneData.Get<Ship>();

  auto mainLayer = Engine().Rendering->InitLayer( //
    shared_from_this(), {0, 0}, Base::Black,
    {
      .Width = 1920,
      .Height = 1080,
    } //
  );
  mainLayer->AddShaderEffect<Bloom>(shared_from_this(), 0.8, 0.05, 0.25);
  AttachLayer<MainLayer>(mainLayer);
}

void DeathScreen::Exit()
{
  auto bus = Base::SignalBus::GetInstance();
  std::shared_ptr<Base::StopAudioStreamSignal> sig = std::make_shared<Base::StopAudioStreamSignal>();
  sig->streamHandle = Engine().Assets->GetAsset<Base::AudioStream>("game-track", true);
  bus->BroadCastSignal(sig);
}
