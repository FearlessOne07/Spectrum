#include "Scenes/DeathScreen/DeathScreen.hpp"
#include "Layers/MainLayer.hpp"

void DeathScreen::Enter(Base::SceneData sceneData)
{
  AttachLayer<MainLayer>(AddRenderLayer({1920, 1080}));
}

void DeathScreen::Exit()
{
}
