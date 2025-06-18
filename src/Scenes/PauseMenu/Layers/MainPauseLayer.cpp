#include "MainPauseLayer.hpp"
#include "base/assets/AssetManager.hpp"
#include "base/scenes/Scene.hpp"
#include "base/ui/UIElement.hpp"
#include "base/ui/UILayer.hpp"
#include "base/ui/UILayoutSettings.hpp"
#include "base/ui/elements/UIButton.hpp"
#include "base/ui/elements/UIContainer.hpp"
#include "raylib.h"
#include <base/renderer/RenderContext.hpp>

void MainPauseLayer::OnInputEvent(std::shared_ptr<Base::InputEvent> &event)
{
  GetOwner()->GetUIManager()->OnInputEvent(event);
}

void MainPauseLayer::OnAttach()
{
  Base::UILayer &mainLayer = GetOwner()->GetUIManager()->AddLayer("main-ui-layer");
  auto container = mainLayer.AddElement<Base::UIContainer>("pause-menu-container");
  container->SetPosition({GetSize().x / 2, -100});
  container->SetPadding({10, 10});
  container->SetGapSize(15);
  container->_onShow = [this, container]() {
    GetOwner()->GetTweenManager()->AddTween<float>( //
      Base::TweenKey{container.get(), "pause-menu-container-position"},
      [container, this](float pos) { container->SetPosition({GetSize().x / 2, pos}); }, container->GetPosition().y,
      GetSize().y / 2, 0.8 //
    );
  };
  container->_onHide = [this, container]() {
    GetOwner()->GetTweenManager()->AddTween<float>( //
      Base::TweenKey{container.get(), "pause-menu-container-position"},
      [container, this](float pos) { container->SetPosition({GetSize().x / 2, pos}); }, container->GetPosition().y,
      -100, .8, Base::TweenManager::EasingType::EASE_IN //
    );
  };

  // Resume Button
  auto resumeButton = container->AddChild<Base::UIButton>("resume-button");
  resumeButton->SetFont(GetOwner()->GetAsset<Base::BaseFont>("main-font-normal"));
  resumeButton->SetText("Resume");
  resumeButton->SetFontSize(50);
  resumeButton->SetLayoutSettings({.hAlignment = Base::UIHAlignment::CENTER});
  resumeButton->onClick = [this, container]() {
    container->Hide();
    GetOwner()->SetSceneTransition(Base::SceneRequest::POP_CURRENT_SCENE);
  };
  resumeButton->onHover = {
    [=, this]() { //
      GetOwner()->GetTweenManager()->AddTween<float>(
        {resumeButton.get(), "font-size"}, [=](float size) { resumeButton->SetFontSize(size); }, 50, 55, 0.1,
        Base::TweenManager::EasingType::EASE_OUT //
      );
    },
    [=, this]() { //
      GetOwner()->GetTweenManager()->AddTween<float>(
        {resumeButton.get(), "font-size"}, [=](float size) { resumeButton->SetFontSize(size); },
        resumeButton->GetFontSize(), 50, 0.1,
        Base::TweenManager::EasingType::EASE_OUT //
      );
    },
  };

  // Exit Button
  auto exitButton = container->AddChild<Base::UIButton>("exit-button");
  exitButton->SetFont(GetOwner()->GetAsset<Base::BaseFont>("main-font-normal"));
  exitButton->SetText("Exit");
  exitButton->SetFontSize(50);
  exitButton->onClick = [this]() { GetOwner()->SetSceneTransition(Base::SceneRequest::QUIT); };
  exitButton->SetLayoutSettings({.hAlignment = Base::UIHAlignment::CENTER});
  exitButton->onHover = {
    [=, this]() { //
      GetOwner()->GetTweenManager()->AddTween<float>(
        {exitButton.get(), "font-size"}, [=](float size) { exitButton->SetFontSize(size); }, 40, 45, 0.1,
        Base::TweenManager::EasingType::EASE_OUT //
      );
    },
    [=, this]() { //
      GetOwner()->GetTweenManager()->AddTween<float>(
        {exitButton.get(), "font-size"}, [=](float size) { exitButton->SetFontSize(size); }, exitButton->GetFontSize(),
        40, 0.1, Base::TweenManager::EasingType::EASE_OUT //
      );
    },
  };

  container->Show();
}

void MainPauseLayer::OnDetach()
{
  GetOwner()->GetUIManager()->RemoveLayer("main-ui-layer");
}

void MainPauseLayer::Render()
{
  GetOwner()->GetUIManager()->RenderLayer("main-ui-layer");
}

void MainPauseLayer::Update(float dt)
{
}
