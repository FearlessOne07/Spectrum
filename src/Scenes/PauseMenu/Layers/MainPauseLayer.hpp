#include "raylib.h"
#include <base/scenes/SceneLayer.hpp>
#include <memory>

class MainPauseLayer : public Base::SceneLayer
{
  Texture _backGroundTexture = {};

public:
  void Update(float dt) override;
  void Render() override;
  void OnAttach() override;
  void OnDetach() override;
  void OnInputEvent(std::shared_ptr<Base::InputEvent> &event) override;
};
