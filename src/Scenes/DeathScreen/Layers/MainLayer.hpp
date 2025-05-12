#pragma once
#include <base/scenes/SceneLayer.hpp>

class MainLayer : public Base::SceneLayer
{

public:
  void Update(float dt) override;
  void Render() override;
  void OnAttach() override;
  void OnDetach() override;
  void OnInputEvent(std::shared_ptr<Base::InputEvent> &event) override;
};
