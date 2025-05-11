#include "GameUILayer.hpp"
#include "base/scenes/Scene.hpp"
#include "base/ui/UILayer.hpp"
#include "base/ui/elements/UIButton.hpp"
#include "base/ui/elements/UILabel.hpp"
#include "raylib.h"
#include <iostream>

void GameUILayer::OnAttach()
{
  Base::UILayer &hud = GetOwner()->GetUIManager()->AddLayer("hud");

  auto fpsLabel = hud.AddElement<Base::UILabel>("fps");
  fpsLabel->SetText("FPS: 0");
  fpsLabel->SetPosition({0, 0});
  fpsLabel->SetFontSize(40);

  auto button = hud.AddElement<Base::UIButton>("noice");
  button->SetText("Press Me");
  button->SetPosition({100, 100});
  button->SetFontSize(35);
  button->onClick = []() { std::cout << "Noice\n"; };
}

void GameUILayer::OnDetach()
{
}

void GameUILayer::OnInputEvent(std::shared_ptr<Base::InputEvent> &event)
{
  GetOwner()->GetUIManager()->OnInputEvent(event);
}

void GameUILayer::Update(float dt)
{
  Base::UILayer &hud = GetOwner()->GetUIManager()->GetLayer("hud");
  auto fpsLabel = hud.GetElement<Base::UILabel>("fps");
  fpsLabel->SetText(TextFormat("FPS: %i", GetFPS()));
}

void GameUILayer::Render()
{
  GetOwner()->GetUIManager()->RenderLayer("hud");
}
