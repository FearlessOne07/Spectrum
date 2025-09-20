#include "Scenes/DeathScreen/DeathScreen.hpp"
#include "Scenes/GameScene/GameScene.hpp"
#include "Scenes/MainMenu/MainMenu.hpp"
#include "Systems/BulletSystem/BulletSystem.hpp"
#include "Systems/DiveSystem/DiveSystem.hpp"
#include "Systems/HealthSystem/HealthSystem.hpp"
#include "Systems/LightCollectionSystem/LightCollectionSystem.hpp"
#include "Systems/TrackingSystem/TrackingSystem.hpp"
#include "Systems/TransformEffectsSystem/TransformEffectsSystem.hpp"
#include <base/game/Game.hpp>

int main(void)
{
  Base::Game game;
  game.Init({
    .Title = "Spectrum",
    .GlobalAssets =
      {
        {"font", "assets/fonts/main-font.ttf"},
        {"audio-stream", "assets/music/game-track.mp3"},
        {"audio-stream", "assets/music/main-menu-track.wav"},
        {"texture", "assets/textures/button.png"},
      },
  });

  // Register Scenes
  game.RegisterScene<MainMenu>(true);
  game.RegisterScene<GameScene>();
  game.RegisterScene<DeathScreen>();

  // Custom Systems
  game.RegisterSystem<BulletSystem>();
  game.RegisterSystem<LightCollectionSystem>();
  game.RegisterSystem<TransformEffectsSystem>();
  game.RegisterSystem<TrackingSystem>();
  game.RegisterSystem<HealthSystem>();
  game.RegisterSystem<DiveSystem>();

  // Run game
  game.Run();
  return 0;
}
