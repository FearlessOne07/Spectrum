#include "Scenes/GameScene/GameScene.hpp"
#include "base/Game.hpp"
#include "base/systems/RenderSystem.hpp"

int main(void)
{
  Base::Game game;
  game.Init(1280, 720, "Spectrum");
  game.RegisterScene<GameScene>();
  game.RegisterSystem<Base::RenderSystem>(true);
  game.Run();
  return 0;
}
