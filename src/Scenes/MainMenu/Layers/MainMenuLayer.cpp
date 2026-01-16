#include "MainMenuLayer.hpp"
#include "Scenes/MainMenu/Signals/ShipSelectionAbortedSignal.hpp"
#include "Scenes/MainMenu/Signals/ShipSelectionStartedSignal.hpp"
#include "base/assets/AssetManager.hpp"
#include "base/scenes/Scene.hpp"
#include "base/signals/SignalBus.hpp"
#include "base/tween/TweenManager.hpp"
#include "base/ui/UIElement.hpp"
#include "base/ui/elements/UIButton.hpp"
#include "base/ui/elements/UIGrid.hpp"
#include "base/ui/elements/UIStackPanel.hpp"
#include <memory>

void MainMenuLayer::OnAttach()
{
  float layerFadeDuration = 0.2;
  _mainMenu = GameCtx().Ui->AddLayer("main-menu", GetSize(), {0, 0}, *this);
  auto mainMenuPanel = _mainMenu->SetLayerBackPanel();
  mainMenuPanel->SetColor(BLACK);
  mainMenuPanel->GetRenderTransform().SetOpacity(1);
  mainMenuPanel->onHide = [mainMenuPanel, this, layerFadeDuration]() {
    GameCtx().Tweens->AddTween<float>(
      {mainMenuPanel.get(), "alpha"},
      [mainMenuPanel](float alpha) { mainMenuPanel->GetRenderTransform().SetOpacity(alpha); },
      {
        .startValue = mainMenuPanel->GetRenderTransform().GetOpacity(),
        .endValue = 0,
        .duration = layerFadeDuration,
        .onTweenEnd = [=]() { mainMenuPanel->SetVisibilityOff(); },
      } //
    );
  };
  mainMenuPanel->onShow = [mainMenuPanel, this, layerFadeDuration]() {
    GameCtx().Tweens->AddTween<float>(
      {mainMenuPanel.get(), "alpha"},
      [mainMenuPanel](float alpha) { mainMenuPanel->GetRenderTransform().SetOpacity(alpha); },
      {
        .startValue = mainMenuPanel->GetRenderTransform().GetOpacity(),
        .endValue = 1,
        .duration = layerFadeDuration,
      } //
    );
  };

  auto container = _mainMenu->SetRootElement<Base::UIStackPanel>();
  container->SetOrientation(Base::UIStackPanel::Orientation::Vertical);
  container->SetHAlignment(Base::HAlign::Center);
  container->SetVAlignment(Base::VAlign::Center);
  container->SetPadding(10);
  container->SetGap(25);
  container->onHide = [this, container, layerFadeDuration]() {
    GameCtx().Tweens->AddTween<float>( //
      {container.get(), "alpha"}, [container](float alpha) { container->GetRenderTransform().SetOpacity(alpha); },
      {
        .startValue = container->GetRenderTransform().GetOpacity(),
        .endValue = 0,
        .duration = layerFadeDuration,
        .onTweenEnd =
          [=]() {
            container->SetVisibilityOff();
            auto bus = Base::SignalBus::GetInstance();
            auto sig = std::make_shared<ShipSelectionStartedSignal>();
            bus->BroadCastSignal(sig);
          },
      } //
    );
  };
  container->onShow = [this, container, layerFadeDuration]() {
    GameCtx().Tweens->AddTween<float>( //
      {container.get(), "alpha"}, [container](float alpha) { container->GetRenderTransform().SetOpacity(alpha); },
      {
        .startValue = container->GetRenderTransform().GetOpacity(),
        .endValue = 1,
        .duration = layerFadeDuration,
      } //
    );
  };

  float hoverScale = 1.15;

  // Resume Button
  auto playButton = container->AddChild<Base::UIButton>("play-button");
  playButton->SetFont(GetOwner()->GameCtx().Assets->GetGlobalAsset<Base::BaseFont>("main-font"));
  playButton->SetText("Play");
  playButton->SetHAlignment(Base::HAlign::Center);
  playButton->SetVAlignment(Base::VAlign::Center);
  playButton->SetFontSize(55);
  playButton->SetPadding(10);
  playButton->onClick = [this]() { _mainMenu->Hide(); };
  // playButton->SetSprite(buttonSprite);
  playButton->SetTextColor(WHITE);
  playButton->SetBackgroundColor(BLANK);

  playButton->onHover = {
    [=, this]() {                        //
      GameCtx().Tweens->AddTween<float>( //
        {playButton.get(), "font-size"}, [=](float size) { playButton->GetRenderTransform().SetFontScale(size); },
        {
          .startValue = playButton->GetRenderTransform().GetFontScale(),
          .endValue = hoverScale,
          .duration = 0.1,
          .easingType = Base::Easings::Type::EaseOut,
        } //
      );
    },
    [=, this]() {                                                                 //
      GameCtx().Tweens->AddTween<float>(                                          //
        {playButton.get(), "font-size"},                                          //
        [=](float size) { playButton->GetRenderTransform().SetFontScale(size); }, //
        {
          .startValue = playButton->GetRenderTransform().GetFontScale(), //
          .endValue = 1,
          .duration = 0.1,
          .easingType = Base::Easings::Type::EaseOut,
        } //
      );
    },
  };

  // Exit Button
  auto exitButton = container->AddChild<Base::UIButton>("exit-button");
  exitButton->SetFont(GetOwner()->GameCtx().Assets->GetGlobalAsset<Base::BaseFont>("main-font"));
  exitButton->SetText("Exit");
  exitButton->SetFontSize(55);
  exitButton->SetHAlignment(Base::HAlign::Center);
  exitButton->SetVAlignment(Base::VAlign::Center);
  exitButton->SetPadding(10);
  exitButton->onClick = [this]() { GetOwner()->SetSceneTransition(Base::SceneRequest::Quit); };
  // exitButton->SetSprite(buttonSprite);
  exitButton->SetTextColor(WHITE);
  exitButton->SetBackgroundColor(BLANK);
  exitButton->onHover = {
    [=, this]() {                        //
      GameCtx().Tweens->AddTween<float>( //
        {exitButton.get(), "font-size"}, [=](float size) { exitButton->GetRenderTransform().SetFontScale(size); },
        {
          .startValue = exitButton->GetRenderTransform().GetFontScale(),
          .endValue = hoverScale,
          .duration = 0.1,
          .easingType = Base::Easings::Type::EaseOut,
        } //
      );
    },
    [=, this]() {                        //
      GameCtx().Tweens->AddTween<float>( //
        {exitButton.get(), "font-size"}, [=](float size) { exitButton->GetRenderTransform().SetFontScale(size); },
        {
          .startValue = exitButton->GetRenderTransform().GetFontScale(),
          .endValue = 1,
          .duration = 0.1,
          .easingType = Base::Easings::Type::EaseOut,
        } //
      );
    },
  };

  auto bus = Base::SignalBus::GetInstance();
  bus->SubscribeSignal<ShipSelectionAbortedSignal>([this](std::shared_ptr<Base::Signal> sig) { _mainMenu->Show(); });
}

void MainMenuLayer::Render()
{
  GameCtx().Ui->RenderLayer(_mainMenu);
}

void MainMenuLayer::Update(float dt)
{
}

void MainMenuLayer::OnDetach()
{
}

void MainMenuLayer::OnInputEvent(std::shared_ptr<Base::InputEvent> &event)
{
  GameCtx().Ui->OnInputEvent(event);
}
