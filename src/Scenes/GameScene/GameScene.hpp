#pragma once
#include "WaveManager/WaveManager.hpp"
#include "base/input/InputEvent.hpp"
#include <base/scenes/Scene.hpp>
#include <memory>

class GameScene : public Base::Scene
{
private:
  WaveManager _waveManager = WaveManager();
  void InitPauseMenu();

  bool _isPaused = false;

public:
  void OnInputEvent(std::shared_ptr<Base::InputEvent> event) override;
  void Enter(Base::SceneData sceneData = Base::SceneData()) override;
  void Suspend() override;
  void Resume() override;
  void Exit() override;

  void Pause();
  void UnPause();
};
