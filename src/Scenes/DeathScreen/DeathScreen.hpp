#pragma once
#include <base/scenes/Scene.hpp>

class DeathScreen : public Base::Scene
{
public:
  void Enter(const Base::SceneData& sceneData = Base::SceneData()) override;
  void Exit() override;
};
