#include "MainGameLayer.hpp"
#include "Components/HealthComponent.hpp"
#include "Components/Tags/PlayerTag.hpp"
#include "Scenes/GameScene/Signals/GamePause.hpp"
#include "Scenes/GameScene/Signals/GameResume.hpp"
#include "Signals/EntityDamagedSignal.hpp"
#include "base/input/InputEvent.hpp"
#include "base/scenes/Scene.hpp"
#include "base/signals/SignalBus.hpp"
#include "base/systems/SystemManager.hpp"
#include <base/renderer/RenderContext.hpp>
#include <base/renderer/RenderContextSingleton.hpp>
#include <memory>

void MainGameLayer::OnAttach()
{
  _waveManager = WaveManager(this, GetOwner()->GetEntityManager());
  _waveManager.SpawnPlayer();
  _playerEVH.Init(this);

  auto bus = Base::SignalBus::GetInstance();
  bus->SubscribeSignal<EntityDamagedSignal>([this](std::shared_ptr<Base::Signal> signal) {
    this->OnPlayerDamaged(signal); //
  });

  bus->SubscribeSignal<GamePausedSignal>([this](std::shared_ptr<Base::Signal>) {
    Pause();
    GetOwner()->SuspendSystems();
  });

  bus->SubscribeSignal<GameResumedSignal>([this](std::shared_ptr<Base::Signal>) {
    UnPause();
    GetOwner()->UnsuspendSystems();
  });

  SetCameraPauseMask();
}

void MainGameLayer::OnDetach()
{
}

void MainGameLayer::OnInputEvent(std::shared_ptr<Base::InputEvent> &event)
{
  GetOwner()->GetSystemManager()->OnInputEvent(event);
}

void MainGameLayer::Update(float dt)
{
  if (!IsPaused())
  {
    _waveManager.SpawnWaves(dt);
  }
}

void MainGameLayer::Render()
{
  const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();
  BeginCamera();
  GetOwner()->GetSystemManager()->Render();
  EndCamera();
}

void MainGameLayer::OnPlayerDamaged(std::shared_ptr<Base::Signal> signal)
{
  if (auto entityDamg = std::dynamic_pointer_cast<EntityDamagedSignal>(signal))
  {
    if (entityDamg->entity->HasComponent<PlayerTag>())
    {
      float health = entityDamg->entity->GetComponent<HealthComponent>()->health;
      float maxHealth = entityDamg->entity->GetComponent<HealthComponent>()->maxHealth;

      GetRenderLayer()->SetShaderUniform( //
        GetOwner()->GetAsset<Base::BaseShader>("vignette"), "u_vignetteStrength",
        float(std::pow((1.f - health / maxHealth), 2)) //
      );
    }
  }
}
