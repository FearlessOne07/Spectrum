#pragma once
#include "Signals/Handlers/EntityEventHandler.hpp"
#include "WaveManager/WaveManager.hpp"
#include "base/ui/UILayer.hpp"
#include <base/scenes/SceneLayer.hpp>

class MainGameLayer : public Base::SceneLayer
{
  WaveManager _waveManager;
  EntitySignalHandler _entityEVH = EntitySignalHandler();

  void OnPlayerDamaged(std::shared_ptr<Base::Signal>);

  // UILayer
  int _currentPopUp = 0;
  Base::Ref<Base::UILayer> _inWorldUILayer;

public:
  void Update(float dt) override;
  void Render() override;
  void OnAttach() override;
  void OnDetach() override;
  void OnInputEvent(std::shared_ptr<Base::InputEvent> &event) override;
};
