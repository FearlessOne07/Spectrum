#pragma once
#include "SpawnManager/SpawnManager.hpp"
#include "base/Scene.hpp"

class GameScene : public Base::Scene
{
private: // Attributes
  SpawnManager _spawnMan = SpawnManager();

private:
  void GetInput() override;

public:
  void Update(float dt, Base::SystemManager *systemManager) override;
  void Enter( //
    Base::SystemManager *systemManager, Base::AssetManager *assetManager,
    Base::SceneData sceneData = Base::SceneData() //
    ) override;
  void Render(Base::SystemManager *systemManager) override;
  void Exit(Base::SystemManager *systemManager, Base::AssetManager *assetManager) override;
};
