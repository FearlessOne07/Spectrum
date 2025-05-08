#include "base/input/InputEvent.hpp"
#include "base/scenes/Scene.hpp"
#include <memory>

class PauseMenu : public Base::Scene
{

private:
public:
  void Enter(Base::SceneData sceneData = Base::SceneData()) override;
  void Exit() override;
  void OnInputEvent(std::shared_ptr<Base::InputEvent> event) override;
};
