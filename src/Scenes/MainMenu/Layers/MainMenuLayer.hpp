#pragma once
#include "Ship/Ship.hpp"
#include "base/ui/UILayer.hpp"
#include <base/scenes/SceneLayer.hpp>

class MainMenuLayer : public Base::SceneLayer
{
  Base::UILayer *_mainMenu = nullptr;
  Base::UILayer *_shipSelection = nullptr;

private:
  void InitMainMenu();
  void InitShipSelection();
  void SelectShip(const Ship &ship);

public:
  void Update(float dt) override;
  void Render() override;
  void OnAttach() override;
  void OnDetach() override;
  void OnInputEvent(std::shared_ptr<Base::InputEvent> &event) override;
};
