#include "MainPauseLayer.hpp"
#include "base/assets/AssetManager.hpp"
#include "base/game/RenderContextSingleton.hpp"
#include "base/scenes/Scene.hpp"
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
  resumeButton->SetPosition( //
    {
      (rd->gameWidth / 2) - (resumeButton->GetSize().x / 2),
      ((rd->gameHeight / 2) - (resumeButton->GetSize().y / 2)) - 100,
    } //
  );
  resumeButton->onClick = [this]() { GetOwner()->SetSceneTransition(Base::SceneRequest::POP_CURRENT_SCENE); };

  // Exit Button
  auto exitButton = mainLayer.AddElement<Base::UIButton>("exit-button");
  exitButton->SetFont(GetOwner()->GetAssetManager()->GetAsset<Font>("main-font-normal"));
  exitButton->SetText("Exit");
  exitButton->SetFontSize(40);
  exitButton->SetPosition( //
    {
      (rd->gameWidth / 2) - (exitButton->GetSize().x / 2),
      ((rd->gameHeight / 2) - (exitButton->GetSize().y / 2)),
    } //
  );
  exitButton->onClick = [this]() { GetOwner()->SetSceneTransition(Base::SceneRequest::QUIT); };
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
