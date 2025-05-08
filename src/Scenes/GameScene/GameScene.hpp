#pragma once
#include "WaveManager/WaveManager.hpp"
#include <base/scenes/Scene.hpp>

class GameScene : public Base::Scene
{
private: // Attributes
private:
  void RegisterSceneEvents();
  WaveManager _waveManager = WaveManager();

public:
  void Enter(Base::SceneData sceneData = Base::SceneData()) override;
  void Exit() override;
};
