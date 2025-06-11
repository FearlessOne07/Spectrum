#pragma once
#include "Signals/Handlers/PlayerEventHandler.hpp"
#include "WaveManager/WaveManager.hpp"
#include <base/scenes/SceneLayer.hpp>

class MainGameLayer : public Base::SceneLayer
{
  WaveManager _waveManager;
  PlayerSignalHandler _playerEVH = PlayerSignalHandler();

  void OnPlayerDamaged(std::shared_ptr<Base::Signal>);

public:
  void Update(float dt) override;
  void Render() override;
  void OnAttach() override;
  void OnDetach() override;
  void OnInputEvent(std::shared_ptr<Base::InputEvent> &event) override;
};
