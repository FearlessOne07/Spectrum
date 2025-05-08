#include "MainPauseLayer.hpp"
#include "base/scenes/SceneTransition.hpp"
#include "raylib.h"

void MainPauseLayer::OnInputEvent(std::shared_ptr<Base::InputEvent> &event)
{
}

void MainPauseLayer::OnAttach()
{
}

void MainPauseLayer::OnDetach()
{
}

void MainPauseLayer::Render()
{
  DrawText("Pause Menu", 0, 0, 40, WHITE);
}

void MainPauseLayer::Update(float dt)
{
}
