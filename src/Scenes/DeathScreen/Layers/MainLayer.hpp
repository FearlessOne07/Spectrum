#pragma once
#include "base/ui/UILayer.hpp"
#include <base/scenes/SceneLayer.hpp>

class MainLayer : public Base::SceneLayer
{
private:
  Base::UILayer *_mainLayer = nullptr;
  float _showMessageTimer = 0;
  float _showActionsTimer = 0;
  bool _messageVisible = false;
  bool _actionsVisible = false;

public:
  void Update(float dt) override;
  void Render() override;
  void OnAttach() override;
  void OnDetach() override;
  void OnInputEvent(std::shared_ptr<Base::InputEvent> &event) override;
};
