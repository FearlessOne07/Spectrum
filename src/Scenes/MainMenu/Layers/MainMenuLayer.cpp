#include "MainMenuLayer.hpp"
#include "Scenes/GameScene/GameScene.hpp"
#include "base/assets/AssetManager.hpp"
#include "base/scenes/Scene.hpp"
#include "base/sprites/NinePatchSprite.hpp"
#include "base/textures/Texture.hpp"
#include "base/tween/TweenManager.hpp"
#include "base/ui/UIElement.hpp"
#include "base/ui/elements/UIButton.hpp"
#include "base/ui/elements/UIGrid.hpp"
#include "base/ui/elements/UIStackPanel.hpp"
#include "base/ui/elements/UITextureRect.hpp"
#include <format>

void MainMenuLayer::OnAttach()
{
  Base::NinePatchSprite buttonSprite = {
    GetAsset<Base::Texture>("button"), {.top = 1, .bottom = 1, .left = 1, .right = 1}, {0, 0}, {16, 8}, 4,
  };

  _mainMenu = GetOwner()->GetUIManager()->AddLayer("main-menu", GetSize());
  auto container = _mainMenu->SetRootElement<Base::UIStackPanel>();
  container->SetOrientation(Base::UIStackPanel::Orientation::Vertical);
  container->SetHAlignment(Base::HAlign::Center);
  container->SetVAlignment(Base::VAlign::Center);
  container->SetPadding(10);
  container->SetGap(25);

  float hoverScale = 1.15;

  // Resume Button
  auto playButton = container->AddChild<Base::UIButton>("play-button");
  playButton->SetFont(GetOwner()->GetAsset<Base::BaseFont>("main-font"));
  playButton->SetText("Play");
  playButton->SetHAlignment(Base::HAlign::Center);
  playButton->SetVAlignment(Base::VAlign::Center);
  playButton->SetFontSize(55);
  playButton->SetPadding(10);
  playButton->onClick = [this]() {
    GetOwner()->SetSceneTransition<GameScene>(Base::SceneRequest::ReplaceCurrentScene);
  };
  playButton->SetSprite(buttonSprite);
  playButton->onHover = {
    [=, this]() {                                     //
      GetOwner()->GetTweenManager()->AddTween<float>( //
        {playButton.get(), "font-size"}, [=](float size) { playButton->GetRenderTransform().SetFontScale(size); },
        {
          .startValue = playButton->GetRenderTransform().GetFontScale(),
          .endValue = hoverScale,
          .duration = 0.1,
          .easingType = Base::TweenManager::EasingType::EaseOut,
        } //
      );
    },
    [=, this]() {                                                                 //
      GetOwner()->GetTweenManager()->AddTween<float>(                             //
        {playButton.get(), "font-size"},                                          //
        [=](float size) { playButton->GetRenderTransform().SetFontScale(size); }, //
        {
          .startValue = playButton->GetRenderTransform().GetFontScale(), //
          .endValue = 1,
          .duration = 0.1,
          .easingType = Base::TweenManager::EasingType::EaseOut,
        } //
      );
    },
  };

  // Exit Button
  auto exitButton = container->AddChild<Base::UIButton>("exit-button");
  exitButton->SetFont(GetOwner()->GetAsset<Base::BaseFont>("main-font"));
  exitButton->SetText("Exit");
  exitButton->SetFontSize(55);
  exitButton->SetHAlignment(Base::HAlign::Center);
  exitButton->SetVAlignment(Base::VAlign::Center);
  exitButton->SetPadding(10);
  exitButton->onClick = [this]() { GetOwner()->SetSceneTransition(Base::SceneRequest::Quit); };
  exitButton->SetSprite(buttonSprite);
  exitButton->onHover = {
    [=, this]() {                                     //
      GetOwner()->GetTweenManager()->AddTween<float>( //
        {exitButton.get(), "font-size"}, [=](float size) { exitButton->GetRenderTransform().SetFontScale(size); },
        {
          .startValue = exitButton->GetRenderTransform().GetFontScale(),
          .endValue = hoverScale,
          .duration = 0.1,
          .easingType = Base::TweenManager::EasingType::EaseOut,
        } //
      );
    },
    [=, this]() {                                     //
      GetOwner()->GetTweenManager()->AddTween<float>( //
        {exitButton.get(), "font-size"}, [=](float size) { exitButton->GetRenderTransform().SetFontScale(size); },
        {
          .startValue = exitButton->GetRenderTransform().GetFontScale(),
          .endValue = 1,
          .duration = 0.1,
          .easingType = Base::TweenManager::EasingType::EaseOut,
        } //
      );
    },
  };
  // _mainMenu->Hide();
}

void MainMenuLayer::Render()
{
  // GetOwner()->GetUIManager()->RenderLayer("main-menu");
}

void MainMenuLayer::Update(float dt)
{
}

void MainMenuLayer::OnDetach()
{
}

void MainMenuLayer::OnInputEvent(std::shared_ptr<Base::InputEvent> &event)
{
  GetOwner()->GetUIManager()->OnInputEvent(event);
}
