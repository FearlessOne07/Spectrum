#include "ShipSelectionLayer.hpp"
#include "Scenes/MainMenu/Signals/ShipSelectionStartedSignal.hpp"
#include "base/assets/AssetManager.hpp"
#include "base/scenes/Scene.hpp"
#include "base/signals/Signal.hpp"
#include "base/signals/SignalBus.hpp"
#include "base/tween/TweenManager.hpp"
#include "base/ui/elements/UIGrid.hpp"
#include "base/ui/elements/UITextureRect.hpp"

void ShipSelectionLayer::OnAttach()
{
  _shipMenu = GetOwner()->GetUIManager()->AddLayer("ship-menu", GetSize(), {0, 0}, *this);
  auto shipGrid = _shipMenu->SetRootElement<Base::UIGrid>();
  shipGrid->SetHAlignment(Base::HAlign::Center);
  shipGrid->SetVAlignment(Base::VAlign::Center);
  shipGrid->SetColumnDefinitions({
    {Base::GridCellSizeMode::Auto},
    {Base::GridCellSizeMode::Auto},
  });
  shipGrid->GetRenderTransform().SetOpacity(0);
  shipGrid->SetVisibilityOff();
  shipGrid->SetPadding(5);
  shipGrid->SetCellGap(100);
  shipGrid->onShow = [this, shipGrid]() {
    GetOwner()->GetTweenManager()->AddTween<float>(
      {shipGrid.get(), "alpha"}, [shipGrid](float alpha) { shipGrid->GetRenderTransform().SetOpacity(alpha); },
      {
        .startValue = shipGrid->GetRenderTransform().GetOpacity(),
        .endValue = 1,
        .duration = 0.6,
      } //
    );
  };

  float hoverScale = 1.15;
  for (int i = 0; i < 1; i++)
  {
    std::string name = std::format("ship-{0}", i);
    auto shipText = shipGrid->AddGridElement<Base::UITextureRect>(name, {i, 0});
    shipText->SetSprite({GetAsset<Base::Texture>("ships"), {}, {i * 8.f, 0}, {8, 8}});
    shipText->SetHAlignment(Base::HAlign::Center);
    shipText->SetVAlignment(Base::VAlign::Center);
    shipText->SetSize({16, 16});
    shipText->onHover = {
      [=, this]() {                                     //
        GetOwner()->GetTweenManager()->AddTween<float>( //
          {shipText.get(), name + "-scale"},
          [=](float size) {
            shipText->GetRenderTransform().SetScaleY(size);
            shipText->GetRenderTransform().SetScaleX(size);
          },
          {
            .startValue = shipText->GetRenderTransform().GetScaleX(),
            .endValue = hoverScale,
            .duration = 0.1,
            .easingType = Base::TweenManager::EasingType::EaseOut,
          } //
        );
      },
      [=, this]() {                                     //
        GetOwner()->GetTweenManager()->AddTween<float>( //
          {shipText.get(), name + "-scale"},
          [=](float size) {
            shipText->GetRenderTransform().SetScaleY(size);
            shipText->GetRenderTransform().SetScaleX(size);
          },
          {
            .startValue = shipText->GetRenderTransform().GetScaleX(),
            .endValue = 1,
            .duration = 0.1,
            .easingType = Base::TweenManager::EasingType::EaseOut,
          } //
        );
      },
    };
  }

  auto bus = Base::SignalBus::GetInstance();
  bus->SubscribeSignal<ShipSelectionStartedSignal>([this](std::shared_ptr<Base::Signal> sig) { _shipMenu->Show(); });
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
