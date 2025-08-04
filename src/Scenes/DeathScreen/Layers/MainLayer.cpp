#include "MainLayer.hpp"
#include "Scenes/GameScene/GameScene.hpp"
#include "Scenes/MainMenu/MainMenu.hpp"
#include "base/assets/AssetManager.hpp"
#include "base/renderer/RenderContext.hpp"
#include "base/renderer/RenderContextSingleton.hpp"
#include "base/scenes/Scene.hpp"
#include "base/tween/TweenManager.hpp"
#include "base/ui/UIElement.hpp"
#include "base/ui/UILayoutSettings.hpp"
#include "base/ui/elements/UIButton.hpp"
#include "base/ui/elements/UIContainer.hpp"
#include "base/ui/elements/UILabel.hpp"
#include "raylib.h"

void MainLayer::OnAttach()
{
  const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();
  _mainLayer = GetOwner()->GetUIManager()->AddLayer("main-layer");

  auto messageContainer = _mainLayer->AddElement<Base::UIContainer>("message-container");
  messageContainer->SetAnchorPoint(Base::UIContainer::AnchorPoint::CENTER);
  messageContainer->SetPosition({GetSize().x / 2, GetSize().y / 2});
  messageContainer->SetPositionalOffset({0, -100});
  messageContainer->SetAlpha(0);
  messageContainer->onShow = [=, this]() {
    GetOwner()->GetTweenManager()->AddTween<float>( //
      {messageContainer.get(), "alpha"}, [=](float alpha) { messageContainer->SetAlpha(alpha); },
      {
        .startValue = 0,
        .endValue = 1,
        .duration = 1,
        .easingType = Base::TweenManager::EasingType::EASE_IN_OUT,
      } //
    );
    GetOwner()->GetTweenManager()->AddTween<Vector2>( //
      {messageContainer.get(), "y-pos-offset"}, [=](Vector2 pos) { messageContainer->SetPositionalOffset(pos); },
      {
        .startValue = messageContainer->GetPositionalOffset(),
        .endValue = {0, 0},
        .duration = 1,
        .easingType = Base::TweenManager::EasingType::EASE_IN_OUT,
      } //
    );
  };

  auto deathLabel = messageContainer->AddChild<Base::UILabel>("death-label");
  deathLabel->SetText("YOU DIED");
  deathLabel->SetFont(GetOwner()->GetAsset<Base::BaseFont>("main-font"));
  deathLabel->SetTextColor(RED);
  deathLabel->SetFontSize(80);
  deathLabel->SetPosition({rd->gameWidth / 2, rd->gameHeight / 2});

  Base::NinePatchSprite buttonSprite = {
    GetAsset<Base::Texture>("button"), {.top = 1, .bottom = 1, .left = 1, .right = 1}, {0, 0}, {16, 8}, 4,
  };

  auto actionButtonContainer = _mainLayer->AddElement<Base::UIContainer>("action-button-container");
  actionButtonContainer->SetLayout(Base::UIContainer::Layout::HORIZONTAL);
  actionButtonContainer->SetPosition({GetSize().x / 2, (GetSize().y / 2) + 400});
  actionButtonContainer->SetAnchorPoint(Base::UIContainer::AnchorPoint::CENTER);
  actionButtonContainer->SetGapMode(Base::UIContainer::GapMode::FIXED);
  actionButtonContainer->SetGapSize(200);
  actionButtonContainer->SetPadding({100, 100});
  actionButtonContainer->SetPositionalOffset({0, 100});
  actionButtonContainer->SetAlpha(0);
  actionButtonContainer->onShow = [=, this]() {
    GetOwner()->GetTweenManager()->AddTween<float>( //
      {actionButtonContainer.get(), "alpha"}, [=](float alpha) { actionButtonContainer->SetAlpha(alpha); },
      {
        .startValue = 0,
        .endValue = 1,
        .duration = 1,
        .easingType = Base::TweenManager::EasingType::EASE_IN_OUT,
      } //
    );
    GetOwner()->GetTweenManager()->AddTween<Vector2>( //
      {actionButtonContainer.get(), "y-pos-offset"},
      [=](Vector2 pos) { actionButtonContainer->SetPositionalOffset(pos); },
      {
        .startValue = actionButtonContainer->GetPositionalOffset(),
        .endValue = {0, 0},
        .duration = 1,
        .easingType = Base::TweenManager::EasingType::EASE_IN_OUT,
      } //
    );
  };

  // Exit Button
  auto mainMenuButton = actionButtonContainer->AddChild<Base::UIButton>("main-menu-button");
  mainMenuButton->SetFont(GetOwner()->GetAsset<Base::BaseFont>("main-font"));
  mainMenuButton->SetText("Quit");
  mainMenuButton->SetFontSize(50);
  mainMenuButton->SetLayoutSettings({.vAlignment = Base::UIVAlignment::CENTER});
  mainMenuButton->onClick = [this]() {
    GetOwner()->SetSceneTransition<MainMenu>(Base::SceneRequest::REPLACE_CURRENT_SCENE);
  };
  mainMenuButton->SetSprite(buttonSprite);
  mainMenuButton->onHover = {
    [=, this]() {                                     //
      GetOwner()->GetTweenManager()->AddTween<float>( //
        {mainMenuButton.get(), "font-size"}, [=](float size) { mainMenuButton->SetFontSize(size, false); },
        {
          .startValue = mainMenuButton->GetFontSize(),
          .endValue = 55,
          .duration = 0.1,
          .easingType = Base::TweenManager::EasingType::EASE_OUT,
        } //
      );
    },
    [=, this]() {                                     //
      GetOwner()->GetTweenManager()->AddTween<float>( //
        {mainMenuButton.get(), "font-size"}, [=](float size) { mainMenuButton->SetFontSize(size, false); },
        {
          .startValue = mainMenuButton->GetFontSize(),
          .endValue = mainMenuButton->GetBaseFontSize(),
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
  playButton->SetLayoutSettings({.vAlignment = Base::UIVAlignment::CENTER});
  playButton->SetSprite(buttonSprite);
  playButton->onHover = {
    [=, this]() {                                     //
      GetOwner()->GetTweenManager()->AddTween<float>( //
        {playButton.get(), "font-size"}, [=](float size) { playButton->SetFontSize(size, false); },
        {
          .startValue = playButton->GetFontSize(),
          .endValue = 60,
          .duration = 0.1,
          .easingType = Base::TweenManager::EasingType::EASE_OUT,
        } //
      );
    },
    [=, this]() {                                                  //
      GetOwner()->GetTweenManager()->AddTween<float>(              //
        {playButton.get(), "font-size"},                           //
        [=](float size) { playButton->SetFontSize(size, false); }, //
        {
          .startValue = playButton->GetFontSize(), //
          .endValue = playButton->GetBaseFontSize(),
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
  if (_showMessageTimer >= 0.5)
  {
    if (!_messageVisible)
    {
      _mainLayer->GetElement<Base::UIContainer>("message-container")->Show();
      _messageVisible = true;
    }

    if (_showActionsTimer >= 1.5)
    {
      if (_messageVisible && !_actionsVisible)
      {
        _mainLayer->GetElement<Base::UIContainer>("action-button-container")->Show();
        _actionsVisible = true;
      }
    }
    else
    {
      _showActionsTimer += dt;
    }
  }
  else
  {
    _showMessageTimer += dt;
  }
}

void MainLayer::OnDetach()
{
  GetOwner()->GetUIManager()->RemoveLayer("main-layer");
}

void MainLayer::OnInputEvent(std::shared_ptr<Base::InputEvent> &event)
{
  GetOwner()->GetUIManager()->OnInputEvent(event);
}
