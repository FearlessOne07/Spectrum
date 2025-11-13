#pragma once
#include "base/ui/UILayer.hpp"
#include "base/util/Ref.hpp"
#include <base/scenes/SceneLayer.hpp>

class MainMenuLayer : public Base::SceneLayer
{
  Base::Ref<Base::UILayer> _mainMenu;
  Base::Ref<Base::UILayer> _shipMenu;

public:
  void Update(float dt) override;
  void Render() override;
  void OnAttach() override;
  void OnDetach() override;
  void OnInputEvent(std::shared_ptr<Base::InputEvent> &event) override;
};
