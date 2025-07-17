#include "MainLayer.hpp"
#include "base/assets/AssetManager.hpp"
#include "base/renderer/RenderContext.hpp"
#include "base/renderer/RenderContextSingleton.hpp"
#include "base/scenes/Scene.hpp"
#include "base/tween/TweenManager.hpp"
#include "base/ui/UIElement.hpp"
#include "base/ui/elements/UILabel.hpp"
#include "raylib.h"

void MainLayer::OnAttach()
{
  const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();
  auto mainLayer = GetOwner()->GetUIManager()->AddLayer("main-layer");

  auto deathLabel = mainLayer->AddElement<Base::UILabel>("death-label");
  deathLabel->SetText("YOU DIED!");
  deathLabel->SetFont(GetOwner()->GetAsset<Base::BaseFont>("main-font"));
  deathLabel->SetTextColor(RED);
  deathLabel->SetFontSize(80);
  deathLabel->SetPosition({rd->gameWidth / 2, rd->gameHeight / 2});

  GetOwner()->GetTweenManager()->AddTween<unsigned char>( //
    {deathLabel.get(), "alpha"}, [=](unsigned char alpha) { deathLabel->SetTextColor({RED.r, RED.g, RED.b, alpha}); },
    {
      .startValue = 0,
      .endValue = 255,
      .duration = 1.5,
      .easingType = Base::TweenManager::EasingType::EASE_IN_OUT,
    } //
  );
  GetOwner()->GetTweenManager()->AddTween<float>( //
    {deathLabel.get(), "font-size"}, [=](float pos) { deathLabel->SetPosition({rd->gameWidth / 2, pos}); },
    {
      .startValue = (rd->gameHeight / 2) - 200,
      .endValue = rd->gameHeight / 2,
      .duration = 1.5,
      .easingType = Base::TweenManager::EasingType::EASE_IN_OUT,
    } //
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
