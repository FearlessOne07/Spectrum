#include "MainLayer.hpp"
#include "base/game/RenderContext.hpp"
#include "base/game/RenderContextSingleton.hpp"
#include "base/scenes/Scene.hpp"
#include "base/ui/elements/UILabel.hpp"

void MainLayer::OnAttach()
{
  const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();
  auto mainLayer = GetOwner()->GetUIManager()->AddLayer("main-layer");
  auto deathLabel = mainLayer.AddElement<Base::UILabel>("death-label");
  deathLabel->SetText("YOU DIED!");
  deathLabel->SetTextColor(RED);
  deathLabel->SetFontSize(70);
  deathLabel->SetPosition({
    (rd->gameWidth / 2) - (deathLabel->GetSize().x / 2),
    (rd->gameHeight / 2) - (deathLabel->GetSize().y / 2),
  });
}

void MainLayer::Render()
{
  GetOwner()->GetUIManager()->RenderLayer("main-layer");
}

void MainLayer::Update(float dt)
{
}

void MainLayer::OnDetach()
{
  GetOwner()->GetUIManager()->RemoveLayer("main-layer");
}

void MainLayer::OnInputEvent(std::shared_ptr<Base::InputEvent> &event)
{
  GetOwner()->GetUIManager()->OnInputEvent(event);
}
