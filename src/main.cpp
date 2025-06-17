#include "Scenes/DeathScreen/DeathScreen.hpp"
#include "Scenes/GameScene/GameScene.hpp"
#include "Scenes/PauseMenu/PauseMenu.hpp"
#include "Systems/BulletSystem/BulletSystem.hpp"
#include "Systems/HealthSystem/HealthSystem.hpp"
#include "Systems/TrackingSystem/TrackingSystem.hpp"
#include "Systems/TransformEffectsSystem/TransformEffectsSystem.hpp"
#include <base/game/Game.hpp>
#include <base/systems/EntityCollisionSystem.hpp>
#include <base/systems/InputSystem.hpp>
#include <base/systems/MoveSystem.hpp>
#include <base/systems/RenderSystem.hpp>

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

  // Register Systems
  game.RegisterSystem<Base::RenderSystem>(true);
  game.RegisterSystem<Base::MoveSystem>();
  game.RegisterSystem<Base::InputSystem>();
  game.RegisterSystem<Base::EntityCollisionSystem>();

  game.RegisterSystem<BulletSystem>();
  game.RegisterSystem<TransformEffectsSystem>();
  game.RegisterSystem<TrackingSystem>();
  game.RegisterSystem<HealthSystem>();

  // Run game
  game.Run();
  return 0;
}
