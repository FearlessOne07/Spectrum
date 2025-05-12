#include "PauseMenu.hpp"
#include "Layers/MainPauseLayer.hpp"

void PauseMenu::OnInputEvent(std::shared_ptr<Base::InputEvent> event)
{
  GetLayerStack().OnInputEvent(event);
}

void PauseMenu::Enter(Base::SceneData sceneData)
{
  GetLayerStack().AttachLayer<MainPauseLayer>();
}

void PauseMenu::Exit()
{
}
