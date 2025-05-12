#include "Scenes/DeathScreen/DeathScreen.hpp"
#include "Layers/MainLayer.hpp"

void DeathScreen::OnInputEvent(std::shared_ptr<Base::InputEvent> event)
{
  GetLayerStack().OnInputEvent(event);
}

void DeathScreen::Enter(Base::SceneData sceneData)
{
  GetLayerStack().AttachLayer<MainLayer>();
}

void DeathScreen::Exit()
{
}
