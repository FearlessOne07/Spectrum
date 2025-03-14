#include "Scenes/GameScene/GameScene.hpp"
#include "Systems/BulletSystem/BulletSystem.hpp"
#include "Systems/GemSystem/GemSystem.hpp"
#include "base/Game.hpp"
#include "base/systems/CameraSystem.hpp"
#include "base/systems/InputSystem.hpp"
#include "base/systems/MoveSystem.hpp"
#include "base/systems/RenderSystem.hpp"

int main(void)
{
  Base::Game game;
  game.Init(1920, 1080, "Spectrum");

  // Register Scenes
  game.RegisterScene<GameScene>();

  // Register Systems
  game.RegisterSystem<Base::RenderSystem>(true);
  game.RegisterSystem<Base::InputSystem>();
  game.RegisterSystem<GemSystem>();
  game.RegisterSystem<BulletSystem>();
  game.RegisterSystem<Base::MoveSystem>();
  game.RegisterSystem<Base::CameraSystem>();
  game.Run();
  return 0;
}
