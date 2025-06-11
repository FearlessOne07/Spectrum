#include "MainGameLayer.hpp"
#include "Components/HealthComponent.hpp"
#include "Components/Tags/PlayerTag.hpp"
#include "Signals/EntityDamagedSignal.hpp"
#include "base/input/InputEvent.hpp"
#include "base/scenes/Scene.hpp"
#include "base/signals/SignalBus.hpp"
#include "base/systems/SystemManager.hpp"
#include <base/renderer/RenderContext.hpp>
#include <base/renderer/RenderContextSingleton.hpp>
#include <cmath>
#include <iostream>
#include <memory>

void MainGameLayer::OnAttach()
{
  _waveManager = WaveManager(GetOwner()->GetEntityManager());
  _waveManager.SpawnPlayer(GetOwner()->GetAssetManager(), GetOwner()->GetCameraManager());
  _playerEVH.Init(GetOwner());

  auto shaderChain = GetRenderLayer()->GetShaderChain();
  shaderChain->AddShaderPass("vignette");
  shaderChain->SetShaderUniform("vignette", "u_resolution", Vector2{1920, 1080});
  std::cout << "ShaderChain address: " << GetRenderLayer()->GetShaderChain() << std::endl;

  auto bus = Base::SignalBus::GetInstance();
  bus->SubscribeSignal<EntityDamagedSignal>([this](std::shared_ptr<Base::Signal> signal) {
    this->OnPlayerDamaged(signal); //
  });
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
  _waveManager.SpawnWaves(dt, GetOwner()->GetCameraManager(), GetOwner()->GetAssetManager());
}

void MainGameLayer::Render()
{
  const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();
  auto camManager = GetOwner()->GetCameraManager();
  camManager->BeginCameraMode();
  GetOwner()->GetSystemManager()->Render();
  camManager->EndCameraMode();
}

void MainGameLayer::OnPlayerDamaged(std::shared_ptr<Base::Signal> signal)
{
  if (auto entityDamg = std::dynamic_pointer_cast<EntityDamagedSignal>(signal))
  {
    if (entityDamg->entity->HasComponent<PlayerTag>())
    {
      float health = entityDamg->entity->GetComponent<HealthComponent>()->health;
      float maxHealth = entityDamg->entity->GetComponent<HealthComponent>()->maxHealth;

      std::cout << "ShaderChain address: " << GetRenderLayer()->GetShaderChain() << std::endl;
      GetRenderLayer()->GetShaderChain()->SetShaderUniform(                              //
        "vignette", "u_vignetteStrength", float(std::pow((1.f - health / maxHealth), 2)) //
      );
    }
  }
}
