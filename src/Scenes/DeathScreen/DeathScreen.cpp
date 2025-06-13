#include "Scenes/DeathScreen/DeathScreen.hpp"
#include "Layers/MainLayer.hpp"

void DeathScreen::OnInputEvent(std::shared_ptr<Base::InputEvent> event)
{
  GetLayerStack().OnInputEvent(event);
}

void DeathScreen::Enter(Base::SceneData sceneData)
{
  LoadAsset<Base::BaseFont>("assets/fonts/main-font-normal.otf");
  GetLayerStack().AttachLayer<MainLayer>(AddRenderLayer({1920, 1080}));
}

void DeathScreen::Exit()
{
}
