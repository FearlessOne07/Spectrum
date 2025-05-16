#include "MainPauseLayer.hpp"
#include "base/assets/AssetManager.hpp"
#include "base/game/RenderContextSingleton.hpp"
#include "base/scenes/Scene.hpp"
#include "base/ui/UIElement.hpp"
#include "base/ui/UILayer.hpp"
#include "base/ui/elements/UIButton.hpp"
#include "raylib.h"
#include <base/game/RenderContext.hpp>

void MainPauseLayer::OnInputEvent(std::shared_ptr<Base::InputEvent> &event)
{
  GetOwner()->GetUIManager()->OnInputEvent(event);
}

void MainPauseLayer::OnAttach()
{
  auto rd = Base::RenderContextSingleton::GetInstance();
  Base::UILayer &mainLayer = GetOwner()->GetUIManager()->AddLayer("main-ui-layer");

  // Resume Button
  auto resumeButton = mainLayer.AddElement<Base::UIButton>("resume-button");
  resumeButton->SetFont(GetOwner()->GetAssetManager()->GetAsset<Font>("main-font-normal"));
  resumeButton->SetText("Resume");
  resumeButton->SetFontSize(40);
  resumeButton->SetAnchorPoint(Base::UIElement::AnchorPoint::CENTER);
  resumeButton->SetPosition( //
    {
      rd->gameWidth / 2,
      (rd->gameHeight / 2) - 50,
    } //
  );
  resumeButton->onClick = [this]() { GetOwner()->SetSceneTransition(Base::SceneRequest::POP_CURRENT_SCENE); };
  resumeButton->onHover = {
    [=, this]() { //
      GetOwner()->GetTweenManager()->AddTween<float>(
        {resumeButton.get(), "font-size"}, [=](float size) { resumeButton->SetFontSize(size); }, 40, 45, 0.1,
        Base::TweenManager::EasingType::EASE_IN_OUT //
      );
    },
    [=, this]() { //
      GetOwner()->GetTweenManager()->AddTween<float>(
        {resumeButton.get(), "font-size"}, [=](float size) { resumeButton->SetFontSize(size); },
        resumeButton->GetFontSize(), 40, 0.1,
        Base::TweenManager::EasingType::EASE_IN_OUT //
      );
    },
  };

  // Exit Button
  auto exitButton = mainLayer.AddElement<Base::UIButton>("exit-button");
  exitButton->SetFont(GetOwner()->GetAssetManager()->GetAsset<Font>("main-font-normal"));
  exitButton->SetText("Exit");
  exitButton->SetFontSize(40);
  exitButton->SetAnchorPoint(Base::UIElement::AnchorPoint::CENTER);
  exitButton->SetPosition( //
    {
      rd->gameWidth / 2,
      (rd->gameHeight / 2) + 50,
    } //
  );
  exitButton->onClick = [this]() { GetOwner()->SetSceneTransition(Base::SceneRequest::QUIT); };
  exitButton->onHover = {
    [=, this]() { //
      GetOwner()->GetTweenManager()->AddTween<float>(
        {exitButton.get(), "font-size"}, [=](float size) { exitButton->SetFontSize(size); }, 40, 45, 0.1,
        Base::TweenManager::EasingType::EASE_IN_OUT //
      );
    },
    [=, this]() { //
      GetOwner()->GetTweenManager()->AddTween<float>(
        {exitButton.get(), "font-size"}, [=](float size) { exitButton->SetFontSize(size); }, exitButton->GetFontSize(),
        40, 0.1,
        Base::TweenManager::EasingType::EASE_IN_OUT //
      );
    },
  };
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
