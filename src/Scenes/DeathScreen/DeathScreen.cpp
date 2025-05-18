#include "Scenes/DeathScreen/DeathScreen.hpp"
#include "Layers/MainLayer.hpp"
#include "base/assets/AssetManager.hpp"

void DeathScreen::OnInputEvent(std::shared_ptr<Base::InputEvent> event)
{
  GetLayerStack().OnInputEvent(event);
}

void DeathScreen::Enter(Base::SceneData sceneData)
{
  GetAssetManager()->LoadAsset<Font>("assets/fonts/main-font-normal.otf");
  GetLayerStack().AttachLayer<MainLayer>();
}

void DeathScreen::Exit()
{
}
