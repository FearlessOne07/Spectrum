#include "MainGameLayer.hpp"
#include "Components/HealthComponent.hpp"
#include "Components/Tags/PlayerTag.hpp"
#include "Scenes/GameScene/SharedGameData.hpp"
#include "Scenes/GameScene/Signals/GamePause.hpp"
#include "Scenes/GameScene/Signals/GameResume.hpp"
#include "ShaderEffects/Vignette/Vignette.hpp"
#include "Signals/EntityDamagedSignal.hpp"
#include "base/components/TransformComponent.hpp"
#include "base/input/InputEvent.hpp"
#include "base/scenes/Scene.hpp"
#include "base/signals/SignalBus.hpp"
#include "base/systems/SystemManager.hpp"
#include "base/ui/UIElement.hpp"
#include "base/ui/elements/UICanvas.hpp"
#include "base/ui/elements/UILabel.hpp"
#include "raylib.h"
#include <base/renderer/RenderContext.hpp>
#include <base/renderer/RenderContextSingleton.hpp>
#include <format>
#include <memory>

void MainGameLayer::OnAttach()
{
  Rectangle worldBounds = {
    GetScreenToWorld({-200, -200}).x,
    GetScreenToWorld({-200, -200}).y,
    GetSize().x + 400,
    GetSize().y + 400,
  };

  GetOwner()->GetEntityManager()->SetWorldBounds(worldBounds);
  _waveManager.Init(this, GetOwner()->GetEntityManager());

  auto sharedData = GetOwner()->GetSharedData<SharedGameData>();
  sharedData->playerId = _waveManager.SpawnPlayer();
  _entityEVH.Init(this);

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

  _inWorldUILayer = GetOwner()->GetUIManager()->AddLayer(                                                   //
    "in-world-ui", {GetSize().x / GetCameraZoom(), GetSize().y / GetCameraZoom()}, GetScreenToWorld({0, 0}) //
  );
  _inWorldUILayer->Hide();

  auto canvas = _inWorldUILayer->SetRootElement<Base::UICanvas>();
  canvas->SetPosition({0, 0});
  canvas->SetVAlignment(Base::VAlign::Stretch);
  canvas->SetHAlignment(Base::HAlign::Stretch);
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

    // Update Vingette
    auto player = GetOwner()->GetEntityManager()->GetEntity(GetOwner()->GetSharedData<SharedGameData>()->playerId);
    if (player)
    {
      auto vignette = GetRenderLayer()->GetShaderEffect<Vignette>();
      auto hlthcmp = player->GetComponent<HealthComponent>();
      if (hlthcmp->GetHealth() <= hlthcmp->GetMaxHealth() * 0.2)
      {
        vignette->SetMinStrength(0.2);
        vignette->SetMaintain(true);
      }
      else
      {
        vignette->SetMinStrength(0);
        vignette->SetMaintain(false);
      }
    }
  }
}

void MainGameLayer::Render()
{
  const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();
  BeginCamera();
  GetOwner()->GetUIManager()->RenderLayer("in-world-ui");
  GetOwner()->GetSystemManager()->Render();
  EndCamera();
}

void MainGameLayer::OnPlayerDamaged(std::shared_ptr<Base::Signal> signal)
{
  auto sig = std::static_pointer_cast<EntityDamagedSignal>(signal);
  if (sig->entity->HasComponent<PlayerTag>())
  {
    Vector2 playerPos = sig->entity->GetComponent<Base::TransformComponent>()->position;
    std::string name = std::format("damage-popup-{}", _currentPopUp++);
    auto canvas = _inWorldUILayer->GetRootElement<Base::UICanvas>();
    auto popUp = canvas->AddChild<Base::UILabel>(name);
    popUp->SetText(std::format("-{0}", sig->damageTaken));
    popUp->SetFont(GetAsset<Base::BaseFont>("main-font"));
    popUp->SetTextColor(RED);
    popUp->SetFontSize(43);
    popUp->SetPosition(GetWorldToScreen(playerPos));
    popUp->SetVAlignment(Base::VAlign::Center);
    popUp->SetHAlignment(Base::HAlign::Center);

    float offsetTarget = 0;
    Vector2 min = GetScreenToWorld({0, 0});
    if (playerPos.y - min.y > 200)
    {
      offsetTarget = -200;
    }
    else
    {
      offsetTarget = 200;
    }

    auto shrink = [this, popUp, name, canvas]() {
      GetOwner()->GetTweenManager()->AddTween<float>(                          //
        {popUp.get(), name + "-font"},                                         //
        [popUp](float pos) { popUp->GetRenderTransform().SetFontScale(pos); }, //
        {
          .startValue = 1,
          .endValue = 0,
          .duration = 0.3,
          .onTweenEnd =
            [this, name, canvas]() {
              if (canvas->HasChild(name))
              {
                canvas->RemoveChild(name);
              }
            },
        } //
      );
    };

    auto rise = [this, popUp, name, shrink, offsetTarget]() {
      GetOwner()->GetTweenManager()->AddTween<float>(                        //
        {popUp.get(), name + "-offsetY"},                                    //
        [popUp](float pos) { popUp->GetRenderTransform().SetOffsetY(pos); }, //
        {
          .startValue = popUp->GetRenderTransform().GetOffsetY(),
          .endValue = offsetTarget,
          .duration = 0.8,
          .onTweenEnd = shrink,
        } //
      );
    };
    rise();

    // Flash Vignette
    auto vignette = GetRenderLayer()->GetShaderEffect<Vignette>();
    vignette->Flash();
  }
}
