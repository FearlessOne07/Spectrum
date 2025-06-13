#include "PauseMenu.hpp"
#include "Layers/MainPauseLayer.hpp"

void PauseMenu::OnInputEvent(std::shared_ptr<Base::InputEvent> event)
{
  GetLayerStack().OnInputEvent(event);
}

void PauseMenu::Enter(Base::SceneData sceneData)
{
  auto mainLayer = AddRenderLayer({1920, 1080});

  GetLayerStack().AttachLayer<MainPauseLayer>(mainLayer);
}

void PauseMenu::Exit()
{
}
