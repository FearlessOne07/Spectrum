#pragma once
#include "Events/Handlers/PlayerEventHandler.hpp"
#include "WaveManager/WaveManager.hpp"
#include "base/input/Events/MouseButtonEvent.hpp"
#include "base/particles/ParticleEmitter.hpp"
#include <base/Scene.hpp>
#include <memory>

class GameScene : public Base::Scene
{
private: // Attributes
  WaveManager _waveManager = WaveManager(GetEntityManager());
  PlayerEventHandler _playerEVH = PlayerEventHandler();
  Base::ParticleEmitter *_emitter;

private:
  void GetInput() override;
  void RegisterSceneEvents();
  void OnMouseEvent(const std::shared_ptr<Base::MouseButtonEvent> &);

public:
  void Update(float dt, Base::SystemManager *systemManager) override;
  void Enter( //
    Base::SystemManager *systemManager, Base::AssetManager *assetManager,
    Base::SceneData sceneData = Base::SceneData() //
    ) override;
  void Render(Base::SystemManager *systemManager) override;
  void Exit(Base::SystemManager *systemManager, Base::AssetManager *assetManager) override;
};
