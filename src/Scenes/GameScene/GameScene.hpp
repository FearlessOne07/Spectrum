#pragma once
#include "Events/Handlers/PlayerEventHandler.hpp"
#include "WaveManager/WaveManager.hpp"
#include <base/scenes/Scene.hpp>

class GameScene : public Base::Scene
{
private: // Attributes
  PlayerEventHandler _playerEVH = PlayerEventHandler();

private:
  void RegisterSceneEvents();
  WaveManager _waveManager = WaveManager();

public:
  void Update(float dt) override;
  void Enter(Base::SceneData sceneData = Base::SceneData()) override;
  void Render() override;
  void Exit() override;
};
