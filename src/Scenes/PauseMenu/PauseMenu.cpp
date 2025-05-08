#include "PauseMenu.hpp"
#include "Layers/MainPauseLayer.hpp"
#include "base/input/Events/KeyEvent.hpp"

void PauseMenu::OnInputEvent(std::shared_ptr<Base::InputEvent> event)
{
  if (auto keyEvent = std::dynamic_pointer_cast<Base::KeyEvent>(event))
  {
    if (keyEvent->key == KEY_ESCAPE && keyEvent->action == Base::InputEvent::Action::PRESSED)
    {
      SetSceneTransition(Base::SceneRequest::POP_CURRENT_SCENE);
    }
  }
}

void PauseMenu::Enter(Base::SceneData sceneData)
{
  GetLayerStack().AttachLayer<MainPauseLayer>();
}

void PauseMenu::Exit()
{
}
