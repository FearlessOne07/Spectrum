#pragma once
#include <base/scenes/Scene.hpp>

class MainMenu : public Base::Scene
{
public:
  void Enter(Base::SceneData sceneData = Base::SceneData()) override;
  void Exit() override;
};
