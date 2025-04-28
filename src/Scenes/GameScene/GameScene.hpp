#pragma once
#include "Events/Handlers/PlayerEventHandler.hpp"
#include "WaveManager/WaveManager.hpp"
#include <base/Scene.hpp>

class GameScene : public Base::Scene
{
private: // Attributes
  WaveManager _waveManager = WaveManager(GetEntityManager());
  PlayerEventHandler _playerEVH = PlayerEventHandler();

private:
  void GetInput() override;
  void RegisterSceneEvents();

public:
  void Update(float dt, Base::SystemManager *systemManager) override;
  void Enter( //
    Base::SystemManager *systemManager, Base::AssetManager *assetManager,
    Base::SceneData sceneData = Base::SceneData() //
    ) override;
  void Render(Base::SystemManager *systemManager) override;
  void Exit(Base::SystemManager *systemManager, Base::AssetManager *assetManager) override;
};
