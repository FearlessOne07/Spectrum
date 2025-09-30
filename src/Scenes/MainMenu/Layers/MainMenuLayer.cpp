#include "MainMenuLayer.hpp"
#include "Scenes/GameScene/GameScene.hpp"
#include "base/assets/AssetManager.hpp"
#include "base/scenes/Scene.hpp"
#include "base/scenes/SceneData.hpp"
#include "base/scenes/SceneTransition.hpp"
#include "base/sprites/NinePatchSprite.hpp"
#include "base/tween/TweenManager.hpp"
#include "base/ui/UIElement.hpp"
#include "base/ui/elements/UIButton.hpp"
#include "base/ui/elements/UIFlexContainer.hpp"
#include "base/ui/elements/UIStackPanel.hpp"
#include "base/ui/elements/UITextureRect.hpp"
#include "raylib.h"
#include <format>
#include <vector>

void MainMenuLayer::OnAttach()
{
  InitMainMenu();
  InitShipSelection();
}

void MainMenuLayer::Render()
{
  GetOwner()->GetUIManager()->RenderLayer("main-menu");
  GetOwner()->GetUIManager()->RenderLayer("ship-selection");
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

void MainMenuLayer::InitMainMenu()
{
  Base::NinePatchSprite buttonSprite = {
    GetAsset<Base::Texture>("button"), {.top = 1, .bottom = 1, .left = 1, .right = 1}, {0, 0}, {16, 8}, 4,
  };

  _mainMenu = GetOwner()->GetUIManager()->AddLayer("main-menu", GetSize());
  _mainMenu->Show();
  auto container = _mainMenu->SetRootElement<Base::UIStackPanel>();
  container->SetOrientation(Base::UIStackPanel::Orientation::Vertical);
  container->SetHAlignment(Base::HAlign::Center);
  container->SetVAlignment(Base::VAlign::Center);
  container->SetPadding(10);
  container->SetGap(25);
  container->onHide = [this, container]() {
    GetOwner()->GetTweenManager()->AddTween<float>(
      {container.get(), "main-menu-alpha"}, [container](float pos) { container->GetRenderTransform().SetOpacity(pos); },
      {
        .startValue = container->GetRenderTransform().GetOpacity(),
        .endValue = 0,
        .duration = 0.6,
        .onTweenEnd = [container, this]() { container->SetVisibilityOff(); },
      } //
    );
  };
  container->onShow = [this, container]() {
    GetOwner()->GetTweenManager()->AddTween<float>(
      {container.get(), "main-menu-alpha"}, [container](float pos) { container->GetRenderTransform().SetOpacity(pos); },
      {
        .startValue = container->GetRenderTransform().GetOpacity(),
        .endValue = 1,
        .duration = 1,
      } //
    );
  };

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
    _mainMenu->Hide();
    _shipSelection->Show();
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
}

void MainMenuLayer::InitShipSelection()
{
  // Ship Selection Menu
  std::vector<std::vector<Vector2>> ships = {
    {
      {24, 0},
      {32, 0},
    },
  };

  const auto &shipTexture = GetAsset<Base::Texture>("entities");

  _shipSelection = GetOwner()->GetUIManager()->AddLayer("ship-selection", GetSize());
  _shipSelection->Hide();

  auto shipStack = _shipSelection->SetRootElement<Base::UIStackPanel>();
  shipStack->SetOrientation(Base::UIStackPanel::Orientation::Horizontal);
  shipStack->GetRenderTransform().SetOpacity(0);
  shipStack->SetVAlignment(Base::VAlign::Center);
  shipStack->SetHAlignment(Base::HAlign::Center);
  shipStack->SetGap(10);
  shipStack->onShow = [=, this]() {
    GetOwner()->GetTweenManager()->AddTween<float>(
      {shipStack.get(), "alpha"}, [shipStack](float pos) { shipStack->GetRenderTransform().SetOpacity(pos); },
      {
        .startValue = shipStack->GetRenderTransform().GetOpacity(),
        .endValue = 1,
        .duration = 0.5,
      } //
    );
  };

  for (int i = 0; i < ships.size(); i++)
  {
    auto row = shipStack->AddChild<Base::UIFlexContainer>(std::format("row-{0}", i));

    row->SetOrientation(Base::UIFlexContainer::Orientation::Vertical);
    row->SetBackgroundColor(RED);

    for (int j = 0; j < ships[i].size(); j++)
    {
      auto ship = row->AddChild<Base::UITextureRect>(std::format("ship-{0}-{1}", i, j));
      ship->SetSize({64, 64});
      ship->SetSprite({
        shipTexture,
        {},
        ships[i][j],
        {8, 8},
      });
      ship->onClick = [=, this]() { SelectShip({shipTexture, ships[i][j]}); };
      ship->SetVAlignment(Base::VAlign::Center);
      ship->SetHAlignment(Base::HAlign::Center);
      ship->onHover = {
        [=, this]() {                                     //
          GetOwner()->GetTweenManager()->AddTween<float>( //
            {ship.get(), "scale"},
            [=](float size) {
              ship->GetRenderTransform().SetScaleX(size);
              ship->GetRenderTransform().SetScaleY(size);
            },
            {
              .startValue = ship->GetRenderTransform().GetScaleX(),
              .endValue = 1.1,
              .duration = 0.1,
              .easingType = Base::TweenManager::EasingType::EaseOut,
            } //
          );
        },
        [=, this]() {                                     //
          GetOwner()->GetTweenManager()->AddTween<float>( //
            {ship.get(), "scale"},                        //
            [=](float size) {
              ship->GetRenderTransform().SetScaleX(size);
              ship->GetRenderTransform().SetScaleY(size);
            }, //
            {
              .startValue = ship->GetRenderTransform().GetScaleX(), //
              .endValue = 1,
              .duration = 0.1,
              .easingType = Base::TweenManager::EasingType::EaseOut,
            } //
          );
        },
      };
    }
  }
}

void MainMenuLayer::SelectShip(const Ship &ship)
{
  Base::SceneData data;
  data.Set<Ship>(ship);
  GetOwner()->SetSceneTransition<GameScene>(Base::SceneRequest::ReplaceCurrentScene, data);
}
