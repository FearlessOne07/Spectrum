#include "MainPauseLayer.hpp"
#include "base/assets/AssetManager.hpp"
#include "base/scenes/Scene.hpp"
#include <base/renderer/RenderContext.hpp>

void MainPauseLayer::OnInputEvent(std::shared_ptr<Base::InputEvent> &event)
{
  GetOwner()->GetUIManager()->OnInputEvent(event);
}

void MainPauseLayer::OnAttach()
{
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
