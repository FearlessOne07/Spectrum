#include "GameScene.hpp"
#include "Layers/GameUILayer.hpp"
#include "Layers/MainGameLayer.hpp"
#include "Layers/ParticleLayer.hpp"
#include "Scenes/PauseMenu/PauseMenu.hpp"
#include "Systems/BulletSystem/BulletSystem.hpp"
#include "Systems/HealthSystem/HealthSystem.hpp"
#include "Systems/TrackingSystem/TrackingSystem.hpp"
#include "Systems/TransformEffectsSystem/TransformEffectsSystem.hpp"
#include "base/audio/Sound.hpp"
#include "base/camera/CameraModes.hpp"
#include "base/input/Events/KeyEvent.hpp"
#include "base/scenes/SceneTransition.hpp"
#include "base/signals/SignalBus.hpp"
#include <base/game/RenderContext.hpp>
#include <base/game/RenderContextSingleton.hpp>
#include <base/systems/EntityCollisionSystem.hpp>
#include <base/systems/InputSystem.hpp>
#include <base/systems/MoveSystem.hpp>
#include <base/systems/RenderSystem.hpp>
#include <base/systems/SystemManager.hpp>
#include <memory>
#include <raylib.h>

void GameScene::Enter(Base::SceneData sceneData)
{
  // Register Events
  auto bus = Base::SignalBus::GetInstance();

  // Init Camera
  const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();
  auto camManager = GetCameraManager();
  camManager->SetCameraOffset({rd->gameWidth / 2, rd->gameHeight / 2});
  camManager->SetCameraZoom(1.3);
  camManager->SetCameraTarget({0, 0});
  camManager->SetCameraRotation(0);
  camManager->SetCameraMode(Base::Camera2DExtMode::STATIC);

  Base::SystemManager *systemManager = GetSystemManager();

  // Activate Systems
  systemManager->ActivatSystem<Base::RenderSystem>();
  systemManager->ActivatSystem<Base::MoveSystem>();
  systemManager->ActivatSystem<Base::InputSystem>();
  systemManager->ActivatSystem<Base::EntityCollisionSystem>();

  systemManager->ActivatSystem<TrackingSystem>();
  systemManager->ActivatSystem<BulletSystem>();
  systemManager->ActivatSystem<HealthSystem>();
  systemManager->ActivatSystem<TransformEffectsSystem>();

  GetAssetManager()->LoadAsset<Texture>("assets/textures/ship.png");
  GetAssetManager()->LoadAsset<Texture>("assets/textures/chaser.png");
  GetAssetManager()->LoadAsset<Texture>("assets/textures/shooter.png");
  GetAssetManager()->LoadAsset<Texture>("assets/textures/shooter-bullet.png");
  GetAssetManager()->LoadAsset<Texture>("assets/textures/player-bullet.png");

  GetAssetManager()->LoadAsset<Font>("assets/fonts/main-font-normal.otf");

  GetAssetManager()->LoadAsset<Base::Sound>("assets/sounds/bullet-fire.wav");
  GetAssetManager()->LoadAsset<Base::Sound>("assets/sounds/enemy-die.wav");

  SetClearColor({7, 7, 15, 255});

  // Layers
  GetLayerStack().AttachLayer<GameUILayer>();
  GetLayerStack().AttachLayer<MainGameLayer>();
  GetLayerStack().AttachLayer<ParticleLayer>();
}

void GameScene::Exit()
{
  GetSystemManager()->DeactivateActiveSystems();
  GetAssetManager()->UnloadAsset<Texture>("ship");
  GetAssetManager()->UnloadAsset<Texture>("chaser");
  GetAssetManager()->UnloadAsset<Texture>("shooter");
  GetAssetManager()->UnloadAsset<Texture>("shooter-bullet");
  GetAssetManager()->UnloadAsset<Texture>("player-bullet");
  GetAssetManager()->UnloadAsset<Font>("main-font-normal");

  GetAssetManager()->UnloadAsset<Base::Sound>("bullet-fire");
  GetAssetManager()->UnloadAsset<Base::Sound>("enemy-die");
}

void GameScene::OnInputEvent(std::shared_ptr<Base::InputEvent> event)
{
  if (auto keyEvent = std::dynamic_pointer_cast<Base::KeyEvent>(event))
  {
    if (keyEvent->key == KEY_ESCAPE && keyEvent->action == Base::InputEvent::Action::PRESSED)
    {
      SetSceneTransition<PauseMenu>(Base::SceneRequest::PUSH_NEW_SCENE);
    }
  }

  GetLayerStack().OnInputEvent(event);
}

void GameScene::Suspend()
{
  GetSystemManager()->SuspendAllSystems();
}

void GameScene::Resume()
{
  GetSystemManager()->UnsuspendSuspendedSystems();
};
