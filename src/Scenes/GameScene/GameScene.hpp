#pragma once
#include "Events/Handlers/PlayerEventHandler.hpp"
#include "Spawner/Spawner.hpp"
#include "WaveManager/WaveManager.hpp"
#include "base/Scene.hpp"
#include <cstddef>

class GameScene : public Base::Scene
{
private: // Attributes
  WaveManager _waveManager = WaveManager(GetEntityManager());
  PlayerEventHandler _playerEVH = PlayerEventHandler();
  size_t _playerID = 0;

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
