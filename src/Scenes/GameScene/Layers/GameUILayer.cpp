#include "GameUILayer.hpp"
#include "Components/HealthComponent.hpp"
#include "Components/Tags/PlayerTag.hpp"
#include "Signals/EntityDamagedSignal.hpp"
#include "Signals/PlayerSpawnedSignal.hpp"
#include "base/assets/AssetManager.hpp"
#include "base/scenes/Scene.hpp"
#include "base/signals/SignalBus.hpp"
#include "base/ui/UILayer.hpp"
#include "base/ui/elements/UILabel.hpp"
#include "raylib.h"
#include <memory>

void GameUILayer::OnAttach()
{
  // Init UI
  Base::UILayer &hud = GetOwner()->GetUIManager()->AddLayer("hud");
  auto fpsLabel = hud.AddElement<Base::UILabel>("fps");
  fpsLabel->SetFont(GetOwner()->GetAsset<Base::BaseFont>("main-font-normal"));
  fpsLabel->SetText("FPS: 0");
  fpsLabel->SetPosition({0, 0});
  fpsLabel->SetFontSize(40);

  auto playerHealth = hud.AddElement<Base::UILabel>("player-health");
  playerHealth->SetFont(GetOwner()->GetAsset<Base::BaseFont>("main-font-normal"));
  playerHealth->SetPosition({0, fpsLabel->GetSize().y});
  playerHealth->SetFontSize(40);

  // Register Event
  auto bus = Base::SignalBus::GetInstance();
  bus->SubscribeSignal<EntityDamagedSignal>([this](std::shared_ptr<Base::Signal> signal) {
    this->OnPlayerDamaged(signal); //
  });

  bus->SubscribeSignal<PlayerSpawnedSignal>([this](std::shared_ptr<Base::Signal> signal) {
    auto spawnSignal = std::dynamic_pointer_cast<PlayerSpawnedSignal>(signal);
    if (spawnSignal)
    {
      auto playerHealth = GetOwner()->GetUIManager()->GetLayer("hud").GetElement<Base::UILabel>("player-health");
      int health = (int)spawnSignal->player->GetComponent<HealthComponent>()->health;
      playerHealth->SetText(TextFormat("Health: %i", health));
    }
  });
}

void GameUILayer::OnDetach()
{
}

void GameUILayer::OnInputEvent(std::shared_ptr<Base::InputEvent> &event)
{
  GetOwner()->GetUIManager()->OnInputEvent(event);
}

void GameUILayer::Update(float dt)
{
  Base::UILayer &hud = GetOwner()->GetUIManager()->GetLayer("hud");
  auto fpsLabel = hud.GetElement<Base::UILabel>("fps");
  fpsLabel->SetText(TextFormat("FPS: %i", GetFPS()));
}

void GameUILayer::Render()
{
  GetOwner()->GetUIManager()->RenderLayer("hud");
}

void GameUILayer::OnPlayerDamaged(std::shared_ptr<Base::Signal> signal)
{
  if (auto entityDamg = std::dynamic_pointer_cast<EntityDamagedSignal>(signal))
  {
    if (entityDamg->entity->HasComponent<PlayerTag>())
    {
      auto playerHealth = GetOwner()->GetUIManager()->GetLayer("hud").GetElement<Base::UILabel>("player-health");
      int health = (int)entityDamg->entity->GetComponent<HealthComponent>()->health;
      playerHealth->SetText(TextFormat("Health: %i", health));
    }
  }
}
