#include "MainMenuLayer.hpp"
#include "Scenes/GameScene/GameScene.hpp"
#include "base/assets/AssetManager.hpp"
#include "base/scenes/Scene.hpp"
#include "base/sprites/NinePatchSprite.hpp"
#include "base/tween/TweenManager.hpp"
#include "base/ui/UIElement.hpp"
#include "base/ui/elements/UIButton.hpp"
#include "base/ui/elements/UIContainer.hpp"
#include "raylib.h"

void MainMenuLayer::OnAttach()
{
  Base::NinePatchSprite buttonSprite = {
    GetAsset<Base::Texture>("button"), {.top = 1, .bottom = 1, .left = 1, .right = 1}, {0, 0}, {16, 8}, 4,
  };

  _mainMenu = GetOwner()->GetUIManager()->AddLayer("main-menu");
  auto container = _mainMenu->AddElement<Base::UIContainer>("main-menu-container");
  container->SetAnchorPoint(Base::UIContainer::AnchorPoint::CENTER);
  container->SetPosition({GetSize().x / 2, GetSize().y / 2});
  container->SetPadding({10, 10});
  container->SetGapSize(25);

  // Resume Button
  auto playButton = container->AddChild<Base::UIButton>("play-button");
  playButton->SetFont(GetOwner()->GetAsset<Base::BaseFont>("main-font"));
  playButton->SetText("Play");
  playButton->SetFontSize(55);
  playButton->SetLayoutSettings({.hAlignment = Base::UIHAlignment::CENTER});
  playButton->onClick = [this]() {
    GetOwner()->SetSceneTransition<GameScene>(Base::SceneRequest::REPLACE_CURRENT_SCENE);
  };
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

  // Exit Button
  auto exitButton = container->AddChild<Base::UIButton>("exit-button");
  exitButton->SetFont(GetOwner()->GetAsset<Base::BaseFont>("main-font"));
  exitButton->SetText("Exit");
  exitButton->SetFontSize(55);
  exitButton->SetLayoutSettings({.hAlignment = Base::UIHAlignment::CENTER});
  exitButton->onClick = [this]() { GetOwner()->SetSceneTransition(Base::SceneRequest::QUIT); };
  exitButton->SetSprite(buttonSprite);
  exitButton->onHover = {
    [=, this]() {                                     //
      GetOwner()->GetTweenManager()->AddTween<float>( //
        {exitButton.get(), "font-size"}, [=](float size) { exitButton->SetFontSize(size, false); },
        {
          .startValue = exitButton->GetFontSize(),
          .endValue = 60,
          .duration = 0.1,
          .easingType = Base::TweenManager::EasingType::EASE_OUT,
        } //
      );
    },
    [=, this]() {                                     //
      GetOwner()->GetTweenManager()->AddTween<float>( //
        {exitButton.get(), "font-size"}, [=](float size) { exitButton->SetFontSize(size, false); },
        {
          .startValue = exitButton->GetFontSize(),
          .endValue = exitButton->GetBaseFontSize(),
          .duration = 0.1,
          .easingType = Base::TweenManager::EasingType::EASE_OUT,
        } //
      );
    },
  };
}

void MainMenuLayer::Render()
{
  GetOwner()->GetUIManager()->RenderLayer("main-menu");
}

void MainMenuLayer::Update(float dt)
{
}

void MainMenuLayer::OnDetach()
{
  GetOwner()->GetUIManager()->RemoveLayer("main-menu");
}

void MainMenuLayer::OnInputEvent(std::shared_ptr<Base::InputEvent> &event)
{
  GetOwner()->GetUIManager()->OnInputEvent(event);
}
