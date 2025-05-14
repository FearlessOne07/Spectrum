#include "MainLayer.hpp"
#include "base/assets/AssetManager.hpp"
#include "base/game/RenderContext.hpp"
#include "base/game/RenderContextSingleton.hpp"
#include "base/scenes/Scene.hpp"
#include "base/tween/TweenManager.hpp"
#include "base/ui/UIElement.hpp"
#include "base/ui/elements/UILabel.hpp"

void MainLayer::OnAttach()
{
  const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();
  auto &mainLayer = GetOwner()->GetUIManager()->AddLayer("main-layer");
  auto deathLabel = mainLayer.AddElement<Base::UILabel>("death-label");
  deathLabel->SetText("YOU DIED!");
  deathLabel->SetFont(GetOwner()->GetAssetManager()->GetAsset<Font>("main-font-normal"));
  deathLabel->SetTextColor(RED);
  deathLabel->SetFontSize(80);
  deathLabel->SetAnchorPoint(Base::UIElement::AnchorPoint::CENTER);
  deathLabel->SetPosition({
    rd->gameWidth / 2,
    rd->gameHeight / 2,
  });

  GetOwner()->GetTweenManager()->AddTween<unsigned char>(
    deathLabel.get() + 1, [=](unsigned char alpha) { deathLabel->SetTextColor({255, 0, 0, alpha}); }, 0, 255, 1);
  GetOwner()->GetTweenManager()->AddTween<float>(
    deathLabel.get() + 2, [=](float pos) { deathLabel->SetPosition({rd->gameWidth / 2, pos}); },
    (rd->gameHeight / 2) - 200, rd->gameHeight / 2, 1 //
  );
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
