#include "MainLayer.hpp"
#include "Scenes/GameScene/GameScene.hpp"
#include "Scenes/MainMenu/MainMenu.hpp"
#include "base/assets/AssetManager.hpp"
#include "base/scenes/Scene.hpp"
#include "base/tween/TweenManager.hpp"
#include "base/ui/UIElement.hpp"
#include "base/ui/elements/UIButton.hpp"
#include "base/ui/elements/UICanvas.hpp"
#include "base/ui/elements/UILabel.hpp"
#include "base/ui/elements/UIStackPanel.hpp"
#include "raylib.h"

void MainLayer::OnAttach()
{
  _mainLayer = GetOwner()->GetUIManager()->AddLayer("main-layer", GetSize());
  auto canvas = _mainLayer->SetRootElement<Base::UICanvas>();
  canvas->SetPosition({0, 0});
  canvas->SetSize({GetSize().x, GetSize().y});

  auto deathMessage = canvas->AddChild<Base::UILabel>("death-message");
  deathMessage->SetText("YOU DIED");
  deathMessage->SetFont(GetOwner()->GetAsset<Base::BaseFont>("main-font"));
  deathMessage->SetPosition({GetSize().x / 2, GetSize().y / 2});
  deathMessage->SetHAlignment(Base::HAlign::Center);
  deathMessage->SetVAlignment(Base::VAlign::Center);
  deathMessage->SetTextColor(RED);
  deathMessage->SetFontSize(80);
  deathMessage->GetRenderTransform().SetOffsetY(-100);
  deathMessage->SetVisibilityOff();
  deathMessage->onShow = [=, this]() {
    GetOwner()->GetTweenManager()->AddTween<float>( //
      {deathMessage.get(), "alpha"}, [=](float alpha) { deathMessage->GetRenderTransform().SetOpacity(alpha); },
      {
        .startValue = 0,
        .endValue = 1,
        .duration = 1.5,
        .easingType = Base::TweenManager::EasingType::EASE_IN_OUT,
      } //
    );
    GetOwner()->GetTweenManager()->AddTween<float>( //
      {deathMessage.get(), "y-pos-offset"}, [=](float pos) { deathMessage->GetRenderTransform().SetOffsetY(pos); },
      {
        .startValue = deathMessage->GetRenderTransform().GetOffsetY(),
        .endValue = 0,
        .duration = 1.5,
        .easingType = Base::TweenManager::EasingType::EASE_IN_OUT,
      } //
    );
  };

  Base::NinePatchSprite buttonSprite = {
    GetAsset<Base::Texture>("button"), {.top = 1, .bottom = 1, .left = 1, .right = 1}, {0, 0}, {16, 8}, 4,
  };

  auto actionButtonContainer = canvas->AddChild<Base::UIStackPanel>("action-button-container");
  actionButtonContainer->SetOrientation(Base::UIStackPanel::Orientation::Horizontal);
  actionButtonContainer->SetPosition({GetSize().x / 2, (GetSize().y / 2) + 400});
  actionButtonContainer->SetHAlignment(Base::HAlign::Center);
  actionButtonContainer->SetVAlignment(Base::VAlign::Center);
  actionButtonContainer->SetGap(200);
  actionButtonContainer->SetPadding(50, 50);
  actionButtonContainer->GetRenderTransform().SetOffsetY(100);
  actionButtonContainer->GetRenderTransform().SetOpacity(0);
  actionButtonContainer->SetVisibilityOff();
  actionButtonContainer->onShow = [=, this]() {
    GetOwner()->GetTweenManager()->AddTween<float>( //
      {actionButtonContainer.get(), "alpha"},
      [=](float alpha) { actionButtonContainer->GetRenderTransform().SetOpacity(alpha); },
      {
        .startValue = 0,
        .endValue = 1,
        .duration = 1,
        .easingType = Base::TweenManager::EasingType::EASE_IN_OUT,
      } //
    );
    GetOwner()->GetTweenManager()->AddTween<float>( //
      {actionButtonContainer.get(), "y-pos-offset"},
      [=](float pos) { actionButtonContainer->GetRenderTransform().SetOffsetY(pos); },
      {
        .startValue = actionButtonContainer->GetRenderTransform().GetOffsetY(),
        .endValue = 0,
        .duration = 1,
        .easingType = Base::TweenManager::EasingType::EASE_IN_OUT,
      } //
    );
  };

  // Exit Button
  float hoverScale = 1.1;
  auto mainMenuButton = actionButtonContainer->AddChild<Base::UIButton>("main-menu-button");
  mainMenuButton->SetFont(GetOwner()->GetAsset<Base::BaseFont>("main-font"));
  mainMenuButton->SetText("Quit");
  mainMenuButton->SetFontSize(50);
  mainMenuButton->SetPadding(10);
  mainMenuButton->SetVAlignment(Base::VAlign::Center);
  mainMenuButton->SetHAlignment(Base::HAlign::Center);
  mainMenuButton->onClick = [this]() {
    GetOwner()->SetSceneTransition<MainMenu>(Base::SceneRequest::REPLACE_CURRENT_SCENE);
  };
  mainMenuButton->SetSprite(buttonSprite);
  mainMenuButton->onHover = {
    [=, this]() {                                     //
      GetOwner()->GetTweenManager()->AddTween<float>( //
        {mainMenuButton.get(), "font-size"},
        [=](float size) { mainMenuButton->GetRenderTransform().SetFontScale(size); },
        {
          .startValue = mainMenuButton->GetRenderTransform().GetFontScale(),
          .endValue = hoverScale,
          .duration = 0.1,
          .easingType = Base::TweenManager::EasingType::EASE_OUT,
        } //
      );
    },
    [=, this]() {                                     //
      GetOwner()->GetTweenManager()->AddTween<float>( //
        {mainMenuButton.get(), "font-size"},
        [=](float size) { mainMenuButton->GetRenderTransform().SetFontScale(size); },
        {
          .startValue = mainMenuButton->GetRenderTransform().GetFontScale(),
          .endValue = 1,
          .duration = 0.1,
          .easingType = Base::TweenManager::EasingType::EASE_OUT,
        } //
      );
    },
  };

  // Play button
  auto playButton = actionButtonContainer->AddChild<Base::UIButton>("play-button");
  playButton->SetFont(GetOwner()->GetAsset<Base::BaseFont>("main-font"));
  playButton->SetText("Retry");
  playButton->SetFontSize(55);
  playButton->onClick = [this]() {
    GetOwner()->SetSceneTransition<GameScene>(Base::SceneRequest::REPLACE_CURRENT_SCENE);
  };
  playButton->SetHAlignment(Base::HAlign::Center);
  playButton->SetVAlignment(Base::VAlign::Center);
  playButton->SetPadding(10);
  playButton->SetSprite(buttonSprite);
  playButton->onHover = {
    [=, this]() {                                     //
      GetOwner()->GetTweenManager()->AddTween<float>( //
        {playButton.get(), "font-size"}, [=](float size) { playButton->GetRenderTransform().SetFontScale(size); },
        {
          .startValue = playButton->GetRenderTransform().GetFontScale(),
          .endValue = hoverScale,
          .duration = 0.1,
          .easingType = Base::TweenManager::EasingType::EASE_OUT,
        } //
      );
    },
    [=, this]() {                                                                                                  //
      GetOwner()->GetTweenManager()->AddTween<float>(                                                              //
        {playButton.get(), "font-size"}, [=](float size) { playButton->GetRenderTransform().SetFontScale(size); }, //
        {
          .startValue = playButton->GetRenderTransform().GetFontScale(),
          .endValue = 1,
          .duration = 0.1,
          .easingType = Base::TweenManager::EasingType::EASE_OUT,
        } //
      );
    },
  };
}

void MainLayer::Render()
{
  GetOwner()->GetUIManager()->RenderLayer("main-layer");
}

void MainLayer::Update(float dt)
{
  if (!_messageVisible)
  {
    _mainLayer->GetRootElement<Base::UICanvas>()->GetChild<Base::UILabel>("death-message")->Show();
    _messageVisible = true;
  }

  if (_showActionsTimer >= 2.5)
  {
    if (_messageVisible && !_actionsVisible)
    {
      _mainLayer->GetRootElement<Base::UICanvas>()->GetChild<Base::UIStackPanel>("action-button-container")->Show();
      _actionsVisible = true;
    }
  }
  else
  {
    _showActionsTimer += dt;
  }
}

void MainLayer::OnDetach()
{
}

void MainLayer::OnInputEvent(std::shared_ptr<Base::InputEvent> &event)
{
  GetOwner()->GetUIManager()->OnInputEvent(event);
}
