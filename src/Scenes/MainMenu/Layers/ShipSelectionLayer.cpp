#include "ShipSelectionLayer.hpp"
#include "Scenes/GameScene/GameScene.hpp"
#include "Scenes/MainMenu/Signals/ShipSelectionAbortedSignal.hpp"
#include "Scenes/MainMenu/Signals/ShipSelectionStartedSignal.hpp"
#include "base/input/Events/KeyEvent.hpp"
#include "base/input/Keys.hpp"
#include "base/scenes/SceneData.hpp"
#include "base/signals/Signal.hpp"
#include "base/signals/SignalBus.hpp"
#include "base/tween/TweenManager.hpp"
#include "base/ui/UIElement.hpp"
#include "base/ui/elements/UIGrid.hpp"
#include "base/ui/elements/UILabel.hpp"
#include "base/ui/elements/UIStackPanel.hpp"
#include "base/ui/elements/UITextureRect.hpp"
#include "base/util/Colors.hpp"
#include <memory>

void ShipSelectionLayer::OnAttach()
{
  float layerFadeDuration = 0.2;
  _shipMenu = GetOwner()->Engine().Ui->AddLayer("ship-menu", GetSize(), {0, 0}, *this);
  auto shipMenuStack = _shipMenu->SetRootElement<Base::UIStackPanel>();
  shipMenuStack->SetBackgroundColor(Base::Red);
  shipMenuStack->SetVisibilityOff();
  shipMenuStack->GetRenderTransform().SetOpacity(0);
  shipMenuStack->onShow = [this, shipMenuStack, layerFadeDuration]() {
    GetOwner()->Engine().Tweens->AddTween<float>(
      {shipMenuStack.get(), "alpha"},
      [shipMenuStack](float alpha) { shipMenuStack->GetRenderTransform().SetOpacity(alpha); },
      {
        .startValue = shipMenuStack->GetRenderTransform().GetOpacity(),
        .endValue = 1,
        .duration = layerFadeDuration,
      } //
    );
  };
  shipMenuStack->onHide = [this, shipMenuStack, layerFadeDuration]() {
    GetOwner()->Engine().Tweens->AddTween<float>(
      {shipMenuStack.get(), "alpha"},
      [shipMenuStack](float alpha) { shipMenuStack->GetRenderTransform().SetOpacity(alpha); },
      {.startValue = shipMenuStack->GetRenderTransform().GetOpacity(),
       .endValue = 0,
       .duration = layerFadeDuration,
       .onTweenEnd =
         [shipMenuStack]() {
           shipMenuStack->SetVisibilityOff();
           auto bus = Base::SignalBus::GetInstance();
           auto sig = std::make_shared<ShipSelectionAbortedSignal>();
           bus->BroadCastSignal(sig);
         }} //
    );
  };
  shipMenuStack->SetHAlignment(Base::HAlign::Stretch);
  shipMenuStack->SetVAlignment(Base::VAlign::Center);
  shipMenuStack->SetGap(20);

  auto shipMenuText = shipMenuStack->AddChild<Base::UILabel>("select-ship-label");
  shipMenuText->SetText("Select A Ship");
  shipMenuText->SetVAlignment(Base::VAlign::Center);
  shipMenuText->SetHAlignment(Base::HAlign::Center);
  shipMenuText->SetFont(GetOwner()->Engine().Assets->GetAsset<Base::Font>("main-font", true));
  shipMenuText->SetFontSize(40);

  auto shipGrid = shipMenuStack->AddChild<Base::UIGrid>("ship-menu-grid");
  shipGrid->SetHAlignment(Base::HAlign::Center);
  shipGrid->SetVAlignment(Base::VAlign::Center);
  shipGrid->SetColumnDefinitions({
    {Base::GridCellSizeMode::Auto},
    {Base::GridCellSizeMode::Auto},
  });
  shipGrid->SetVisibilityOff();
  shipGrid->SetPadding(20);
  shipGrid->SetCellGap(40);

  float hoverScale = 1.15;
  for (int i = 0; i < 2; i++)
  {
    std::string name = std::format("ship-{0}", i);
    auto shipText = shipGrid->AddGridElement<Base::UITextureRect>(name, {i, 0});
    shipText->SetSprite(
      {GetOwner()->Engine().Assets->GetAsset<Base::Texture>("ships", true), {}, {i * 8.f, 0}, {8, 8}});
    shipText->SetHAlignment(Base::HAlign::Center);
    shipText->SetVAlignment(Base::VAlign::Center);
    shipText->SetSize({16 * 4, 16 * 4});
    shipText->onClick = [this, i]() {
      Base::SceneData data;
      data.Set(Ship({{i * 8.f, 0}, {8, 8}}));
      GetOwner()->SetSceneTransition<GameScene>(Base::SceneRequest::ReplaceCurrentScene, data);
    };
    shipText->onHover = {
      [=, this]() {                                   //
        GetOwner()->Engine().Tweens->AddTween<float>( //
          {shipText.get(), name + "-scale"},
          [=](float size) {
            shipText->GetRenderTransform().SetScaleY(size);
            shipText->GetRenderTransform().SetScaleX(size);
          },
          {
            .startValue = shipText->GetRenderTransform().GetScaleX(),
            .endValue = hoverScale,
            .duration = 0.1,
            .easingType = Base::Easings::Type::EaseOut,
          } //
        );
      },
      [=, this]() {                                   //
        GetOwner()->Engine().Tweens->AddTween<float>( //
          {shipText.get(), name + "-scale"},
          [=](float size) {
            shipText->GetRenderTransform().SetScaleY(size);
            shipText->GetRenderTransform().SetScaleX(size);
          },
          {
            .startValue = shipText->GetRenderTransform().GetScaleX(),
            .endValue = 1,
            .duration = 0.1,
            .easingType = Base::Easings::Type::EaseOut,
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
  GetOwner()->Engine().Ui->RenderLayer(_shipMenu);
}

void ShipSelectionLayer::Update(float dt)
{
}

void ShipSelectionLayer::OnDetach()
{
}

void ShipSelectionLayer::OnInputEvent(std::shared_ptr<Base::InputEvent> &event)
{
  if (auto keyEvent = std::dynamic_pointer_cast<Base::KeyEvent>(event))
  {
    if (keyEvent->Key == Base::Key::Escape && keyEvent->action == Base::KeyEvent::Action::Pressed)
    {
      _shipMenu->Hide();
      keyEvent->isHandled = true;
    }
  }
  GetOwner()->Engine().Ui->OnInputEvent(event);
}
