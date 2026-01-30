#include "Assets/GlobalAssets.hpp"
#include "Scenes/DeathScreen/DeathScreen.hpp"
#include "Scenes/GameScene/GameScene.hpp"
#include "Scenes/MainMenu/MainMenu.hpp"
#include "Systems/BulletSystem/BulletSystem.hpp"
#include "Systems/CollectionSystem/CollectionSystem.hpp"
#include "Systems/DiveSystem/DiveSystem.hpp"
#include "Systems/HealthSystem/HealthSystem.hpp"
#include "Systems/TrackingSystem/TrackingSystem.hpp"
#include "Systems/TransformEffectsSystem/TransformEffectsSystem.hpp"
#include <base/game/Game.hpp>

int main(void)
{
  Base::Game game;
  game.Init({
    .Title = "Spectrum",
  });

  game.LoadGlobalAssets<GlobalAssets>();

  // Register Scenes
  game.RegisterScene<MainMenu>(true);
  game.RegisterScene<GameScene>();
  game.RegisterScene<DeathScreen>();

  // Custom Systems
  game.RegisterSystem<BulletSystem>();
  game.RegisterSystem<CollectionSystem>();
  game.RegisterSystem<TransformEffectsSystem>();
  game.RegisterSystem<TrackingSystem>();
  game.RegisterSystem<HealthSystem>();
  game.RegisterSystem<DiveSystem>();

  // Run game
  game.Run();
  return 0;
}
