#include "ShipSelectionLayer.hpp"
#include "base/assets/AssetManager.hpp"
#include "base/scenes/Scene.hpp"
#include "base/tween/TweenManager.hpp"
#include "base/ui/elements/UIGrid.hpp"
#include "base/ui/elements/UITextureRect.hpp"

void ShipSelectionLayer::OnAttach()
{
  _shipMenu = GetOwner()->GetUIManager()->AddLayer("ship-menu", GetSize());
  auto shipGrid = _shipMenu->SetRootElement<Base::UIGrid>();
  shipGrid->SetHAlignment(Base::HAlign::Center);
  shipGrid->SetVAlignment(Base::VAlign::Center);

  shipGrid->SetColumnDefinitions({
    {Base::GridCellSizeMode::Auto},
    {Base::GridCellSizeMode::Auto},
  });

  for (int i = 0; i < 2; i++)
  {
    auto shipText = shipGrid->AddGridElement<Base::UITextureRect>(std::format("ship-{0}", i), {i, 0});
    shipText->SetSprite({GetAsset<Base::Texture>("ships"), {}, {i * 8.f, 0}, {8, 8}});
    shipText->SetSize({16, 16});
  }
}

void ShipSelectionLayer::Render()
{
  GetOwner()->GetUIManager()->RenderLayer("ship-menu");
}

void ShipSelectionLayer::Update(float dt)
{
}

void ShipSelectionLayer::OnDetach()
{
}

void ShipSelectionLayer::OnInputEvent(std::shared_ptr<Base::InputEvent> &event)
{
  GetOwner()->GetUIManager()->OnInputEvent(event);
}
