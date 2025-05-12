#pragma once
#include "base/input/InputEvent.hpp"
#include <base/scenes/Scene.hpp>
#include <memory>

class DeathScreen : public Base::Scene
{
public:
  void OnInputEvent(std::shared_ptr<Base::InputEvent> event) override;
  void Enter(Base::SceneData sceneData = Base::SceneData()) override;
  void Exit() override;
};
