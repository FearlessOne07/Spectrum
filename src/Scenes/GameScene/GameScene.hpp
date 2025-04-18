#pragma once
#include "Events/Handlers/PlayerEventHandler.hpp"
#include "SpawnManager/SpawnManager.hpp"
#include "base/Scene.hpp"
#include <cstddef>

class GameScene : public Base::Scene
{
private: // Attributes
  SpawnManager _spawnMan = SpawnManager();
  size_t _playerID = 0;
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
