#pragma once
#include "Ship/ShipDataBase.hpp"
#include "base/ui/UILayer.hpp"
#include "base/util/Ref.hpp"
#include <array>
#include <base/scenes/SceneLayer.hpp>
#include <memory>

class ShipSelectionLayer : public Base::SceneLayer
{
  Base::Ref<Base::UILayer> _shipMenu;
  std::array<ShipName, 2> _ships = {ShipName::White, ShipName::Yellow};

public:
  void Update(float dt) override;
  void Render() override;
  void OnAttach() override;
  void OnDetach() override;
  void OnInputEvent(std::shared_ptr<Base::InputEvent> &event) override;
};
