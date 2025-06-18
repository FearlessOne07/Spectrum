#include "Scenes/DeathScreen/DeathScreen.hpp"
#include "Scenes/GameScene/GameScene.hpp"
#include "Scenes/PauseMenu/PauseMenu.hpp"
#include "Systems/BulletSystem/BulletSystem.hpp"
#include "Systems/HealthSystem/HealthSystem.hpp"
#include "Systems/PowerUpSystem/PowerUpSystem.hpp"
#include "Systems/TrackingSystem/TrackingSystem.hpp"
#include "Systems/TransformEffectsSystem/TransformEffectsSystem.hpp"
#include <base/game/Game.hpp>

int main(void)
{
  Base::Game game;
  game.Init({
    .Title = "Spectrum",
    .globalAssets =
      {
        {"font", "assets/fonts/main-font-normal.ttf"},
        {"audio-stream", "assets/music/main-track.mp3"},
      },
  });

  // Register Scenes
  game.RegisterScene<GameScene>(true);
  game.RegisterScene<PauseMenu>();
  game.RegisterScene<DeathScreen>();

  // Custom Systems
  game.RegisterSystem<BulletSystem>();
  game.RegisterSystem<TransformEffectsSystem>();
  game.RegisterSystem<TrackingSystem>();
  game.RegisterSystem<HealthSystem>();
  game.RegisterSystem<PowerUpSystem>();

  // Run game
  game.Run();
  return 0;
}
