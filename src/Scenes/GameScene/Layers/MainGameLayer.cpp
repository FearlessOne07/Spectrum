#include "MainGameLayer.hpp"
#include "Components/Tags/PlayerTag.hpp"
#include "Scenes/GameScene/SharedGameData.hpp"
#include "Scenes/GameScene/Signals/GamePause.hpp"
#include "Scenes/GameScene/Signals/GameResume.hpp"
#include "Signals/EntityDamagedSignal.hpp"
#include "base/components/TransformComponent.hpp"
#include "base/input/InputEvent.hpp"
#include "base/scenes/Scene.hpp"
#include "base/signals/SignalBus.hpp"
#include "base/systems/SystemManager.hpp"
#include "base/ui/elements/UILabel.hpp"
#include "base/ui/elements/UIStackPanel.hpp"
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

  _inWorldUILayer = GetOwner()->GetUIManager()->AddLayer("in-world-ui", GetSize());
  _inWorldUILayer->Hide();
}

void MainGameLayer::OnDetach()
{
  GetOwner()->GetUIManager()->RemoveLayer("in-world-ui");
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
  // GetOwner()->GetUIManager()->RenderLayer("in-world-ui");
  EndCamera();
}

void MainGameLayer::OnPlayerDamaged(std::shared_ptr<Base::Signal> signal)
{
  // auto sig = std::static_pointer_cast<EntityDamagedSignal>(signal);
  // if (sig->entity->HasComponent<PlayerTag>())
  // {
  //
  //   Vector2 playerPos = sig->entity->GetComponent<Base::TransformComponent>()->position;
  //
  //   std::string name = std::format("damage-pop-cont-{0}", _currentPopUp++);
  //   auto popUpCont = _inWorldUILayer->AddElement<Base::UIContainer>(name);
  //   popUpCont->SetAnchorPoint(Base::UIContainer::AnchorPoint::CENTER);
  //   popUpCont->SetPosition(playerPos, true);
  //
  //   auto popUp = popUpCont->AddChild<Base::UILabel>(name + "-popup");
  //   popUp->SetText(std::format("-{0}", sig->damageTaken));
  //   popUp->SetFont(GetAsset<Base::BaseFont>("main-font"));
  //   popUp->SetTextColor(RED);
  //   popUp->SetFontSize(43);
  //
  //   float offsetTarget = 0;
  //   Vector2 min = GetScreenToWorld({0, 0});
  //   if (playerPos.y - min.y > 200)
  //   {
  //     offsetTarget = -200;
  //   }
  //   else
  //   {
  //     offsetTarget = 200;
  //   }
  //
  //   auto shrink = [this, popUp, name]() {
  //     GetOwner()->GetTweenManager()->AddTween<float>(          //
  //       {popUp.get(), name + "-popup-" + "font"},              //
  //       [popUp](float pos) { popUp->SetFontSize(pos, true); }, //
  //       {
  //         .startValue = 43,
  //         .endValue = 0,
  //         .duration = 0.3,
  //         .onTweenEnd =
  //           [this, name]() {
  //             if (_inWorldUILayer->HasElement(name))
  //             {
  //               _inWorldUILayer->RemoveElement(name);
  //             }
  //           },
  //       } //
  //     );
  //   };
  //
  //   auto rise = [this, popUpCont, name, shrink, offsetTarget]() {
  //     GetOwner()->GetTweenManager()->AddTween<Vector2>(                    //
  //       {popUpCont.get(), name + "-postionoffset"},                        //
  //       [popUpCont](Vector2 pos) { popUpCont->SetPositionalOffset(pos); }, //
  //       {
  //         .startValue = {0, 0},
  //         .endValue = {0, offsetTarget},
  //         .duration = 0.8,
  //         .onTweenEnd = shrink,
  //       } //
  //     );
  //   };
  //   rise();
  // }
}
