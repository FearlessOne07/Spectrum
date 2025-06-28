#include "GameUILayer.hpp"
#include "Components/HealthComponent.hpp"
#include "Components/LightCollectorComponent.hpp"
#include "Components/Tags/PlayerTag.hpp"
#include "Scenes/GameScene/Signals/GamePause.hpp"
#include "Scenes/GameScene/Signals/GameResume.hpp"
#include "Signals/EntityDamagedSignal.hpp"
#include "Signals/PlayerSpawnedSignal.hpp"
#include "base/assets/AssetManager.hpp"
#include "base/entities/EntityManager.hpp"
#include "base/input/Events/KeyEvent.hpp"
#include "base/scenes/Scene.hpp"
#include "base/signals/SignalBus.hpp"
#include "base/ui/UILayer.hpp"
#include "base/ui/elements/UIButton.hpp"
#include "base/ui/elements/UIContainer.hpp"
#include "base/ui/elements/UILabel.hpp"
#include "base/ui/elements/UIPanel.hpp"
#include "base/ui/elements/UITextureRect.hpp"
#include "raylib.h"
#include <memory>

void GameUILayer::OnAttach()
{
  // Init UI
  InitHud();
  InitPauseMenu();

  // Register Events
  auto bus = Base::SignalBus::GetInstance();
  bus->SubscribeSignal<EntityDamagedSignal>([this](std::shared_ptr<Base::Signal> signal) {
    this->OnPlayerDamaged(signal); //
  });

  bus->SubscribeSignal<PlayerSpawnedSignal>([this](std::shared_ptr<Base::Signal> signal) {
    auto spawnSignal = std::dynamic_pointer_cast<PlayerSpawnedSignal>(signal);
    if (spawnSignal)
    {
      auto playerHealth =
        _hud->GetElement<Base::UIContainer>("player-health-container")->GetChild<Base::UILabel>("player-health");
      int health = (int)spawnSignal->player->GetComponent<HealthComponent>()->health;
      playerHealth->SetText(TextFormat("%i", health));
    }
  });
}

void GameUILayer::OnDetach()
{
}

void GameUILayer::OnInputEvent(std::shared_ptr<Base::InputEvent> &event)
{
  if (auto keyEvent = std::dynamic_pointer_cast<Base::KeyEvent>(event))
  {
    auto bus = Base::SignalBus::GetInstance();
    auto pSig = std::make_shared<GamePausedSignal>();
    auto rSig = std::make_shared<GameResumedSignal>();
    if (keyEvent->key == KEY_ESCAPE && keyEvent->action == Base::InputEvent::Action::PRESSED)
    {
      if (_pauseMenu->IsVisible())
      {
        _pauseMenu->Hide();
        bus->BroadCastSignal(rSig);
      }
      else
      {
        bus->BroadCastSignal(pSig);
        _pauseMenu->Show();
      }
      event->isHandled = true;
    }
  }
  GetOwner()->GetUIManager()->OnInputEvent(event);
}

void GameUILayer::Update(float dt)
{
  auto player = GetOwner()->GetEntityManager()->Query<PlayerTag>();
  auto lightcmp = player[0]->item->GetComponent<LightCollectorComponent>();

  auto playerLight = _hud->GetElement<Base::UIContainer>("light-container")->GetChild<Base::UILabel>("player-light");
  playerLight->SetText(TextFormat("%i", lightcmp->value));
}

void GameUILayer::Render()
{
  GetOwner()->GetUIManager()->RenderLayer("hud");
  GetOwner()->GetUIManager()->RenderLayer("pause-menu");
}

void GameUILayer::OnPlayerDamaged(std::shared_ptr<Base::Signal> signal)
{
  if (auto entityDamg = std::dynamic_pointer_cast<EntityDamagedSignal>(signal))
  {
    if (entityDamg->entity->HasComponent<PlayerTag>())
    {
      auto playerHealth =
        _hud->GetElement<Base::UIContainer>("player-health-container")->GetChild<Base::UILabel>("player-health");
      int health = (int)entityDamg->entity->GetComponent<HealthComponent>()->health;
      playerHealth->SetText(TextFormat("%i", health));
    }
  }
}

void GameUILayer::InitPauseMenu()
{
  auto bus = Base::SignalBus::GetInstance();
  auto pSig = std::make_shared<GamePausedSignal>();
  auto rSig = std::make_shared<GameResumedSignal>();

  _pauseMenu = GetOwner()->GetUIManager()->AddLayer("pause-menu");
  auto container = _pauseMenu->AddElement<Base::UIContainer>("pause-menu-container");
  container->SetAnchorPoint(Base::UIContainer::AnchorPoint::CENTER);
  container->SetPosition({GetSize().x / 2, -100});
  container->SetPadding({10, 10});
  container->SetGapSize(15);
  container->_onShow = [this, container]() {
    GetOwner()->GetTweenManager()->AddTween<float>( //
      Base::TweenKey{container.get(), "pause-menu-container-position"},
      [container, this](float pos) { container->SetPosition({GetSize().x / 2, pos}); }, container->GetBasePosition().y,
      GetSize().y / 2, 0.5 //
    );
  };
  container->_onHide = [this, container]() {
    GetOwner()->GetTweenManager()->AddTween<float>( //
      Base::TweenKey{container.get(), "pause-menu-container-position"},
      [container, this](float pos) { container->SetPosition({GetSize().x / 2, pos}); }, container->GetBasePosition().y,
      -100, .3, Base::TweenManager::EasingType::EASE_IN //
    );
  };

  // Resume Button
  auto resumeButton = container->AddChild<Base::UIButton>("resume-button");
  resumeButton->SetFont(GetOwner()->GetAsset<Base::BaseFont>("main-font"));
  resumeButton->SetText("Resume");
  resumeButton->SetFontSize(50);
  resumeButton->SetLayoutSettings({.hAlignment = Base::UIHAlignment::CENTER});
  resumeButton->onClick = [this, bus, rSig]() {
    _pauseMenu->Hide();
    bus->BroadCastSignal(rSig);
  };
  resumeButton->onHover = {
    [=, this]() { //
      GetOwner()->GetTweenManager()->AddTween<float>(
        {resumeButton.get(), "font-size"}, [=](float size) { resumeButton->SetFontSize(size, false); },
        resumeButton->GetFontSize(), 55, 0.1,
        Base::TweenManager::EasingType::EASE_OUT //
      );
    },
    [=, this]() { //
      GetOwner()->GetTweenManager()->AddTween<float>(
        {resumeButton.get(), "font-size"}, [=](float size) { resumeButton->SetFontSize(size, false); },
        resumeButton->GetFontSize(), resumeButton->GetBaseFontSize(), 0.1,
        Base::TweenManager::EasingType::EASE_OUT //
      );
    },
  };

  // Exit Button
  auto exitButton = container->AddChild<Base::UIButton>("exit-button");
  exitButton->SetFont(GetOwner()->GetAsset<Base::BaseFont>("main-font"));
  exitButton->SetText("Exit");
  exitButton->SetFontSize(50);
  exitButton->SetLayoutSettings({.hAlignment = Base::UIHAlignment::CENTER});
  exitButton->onClick = [this]() { GetOwner()->SetSceneTransition(Base::SceneRequest::QUIT); };
  exitButton->onHover = {
    [=, this]() { //
      GetOwner()->GetTweenManager()->AddTween<float>(
        {exitButton.get(), "font-size"}, [=](float size) { exitButton->SetFontSize(size, false); },
        exitButton->GetFontSize(), 55, 0.1,
        Base::TweenManager::EasingType::EASE_OUT //
      );
    },
    [=, this]() { //
      GetOwner()->GetTweenManager()->AddTween<float>(
        {exitButton.get(), "font-size"}, [=](float size) { exitButton->SetFontSize(size, false); },
        exitButton->GetFontSize(), exitButton->GetBaseFontSize(), 0.1, Base::TweenManager::EasingType::EASE_OUT //
      );
    },
  };

  auto pauseMenuPanel = _pauseMenu->AddElement<Base::UIPanel>("pause-menu-panel");
  pauseMenuPanel->SetPosition({0, 0});
  Color panelColor = {7, 7, 10, 255};
  panelColor = {panelColor.r, panelColor.g, panelColor.b, 0};
  pauseMenuPanel->SetColor(panelColor);
  pauseMenuPanel->SetSize(GetSize());
  pauseMenuPanel->_onHide = [=, this]() {
    GetOwner()->GetTweenManager()->AddTween<unsigned char>( //
      Base::TweenKey{pauseMenuPanel.get(), "pause-menu-panel-alpha"},
      [this, pauseMenuPanel, panelColor](unsigned char alpha) {
        pauseMenuPanel->SetColor({panelColor.r, panelColor.g, panelColor.b, alpha});
      },
      pauseMenuPanel->GetColor().a, 0, 0.3 //
    );
  };

  pauseMenuPanel->_onShow = [=, this]() {
    GetOwner()->GetTweenManager()->AddTween<unsigned char>( //
      Base::TweenKey{pauseMenuPanel.get(), "pause-menu-panel-alpha"},
      [this, pauseMenuPanel, panelColor](unsigned char alpha) {
        pauseMenuPanel->SetColor({panelColor.r, panelColor.g, panelColor.b, alpha});
      },
      pauseMenuPanel->GetColor().a, 200, 0.5 //
    );
  };
  _pauseMenu->Hide();
}

void GameUILayer::InitHud()
{
  _hud = GetOwner()->GetUIManager()->AddLayer("hud");
  auto healtContainer = _hud->AddElement<Base::UIContainer>("player-health-container");
  healtContainer->SetPosition({0, 0});
  healtContainer->SetLayout(Base::UIContainer::Layout::HORIZONTAL);
  healtContainer->SetGapSize(20);
  healtContainer->SetPadding({15, 10});

  auto heartIcon = healtContainer->AddChild<Base::UITextureRect>("heart-icon");
  heartIcon->SetTextureHandle(GetOwner()->GetAsset<Base::Texture>("heart-ui"));
  heartIcon->SetSize({40, 40});
  heartIcon->SetSourceRect({2 * 8, 0, 8, 8});
  heartIcon->SetLayoutSettings({.vAlignment = Base::UIVAlignment::CENTER});

  auto playerHealth = healtContainer->AddChild<Base::UILabel>("player-health");
  playerHealth->SetFont(GetOwner()->GetAsset<Base::BaseFont>("main-font"));
  playerHealth->SetFontSize(40);
  playerHealth->SetLayoutSettings({.vAlignment = Base::UIVAlignment::CENTER});

  auto lightContainer = _hud->AddElement<Base::UIContainer>("light-container");
  lightContainer->SetLayout(Base::UIContainer::Layout::HORIZONTAL);
  lightContainer->SetGapSize(20);
  lightContainer->SetPadding({15, 10});
  lightContainer->SetPosition({0, 60});

  auto lightIcon = lightContainer->AddChild<Base::UITextureRect>("light-icon");
  lightIcon->SetSourceRect({2 * 8, 1 * 8, 8, 8});
  lightIcon->SetTextureHandle(GetAsset<Base::Texture>("power-ups"));
  lightIcon->SetLayoutSettings({.vAlignment = Base::UIVAlignment::CENTER});
  lightIcon->SetSize({40, 40});

  auto playerLight = lightContainer->AddChild<Base::UILabel>("player-light");
  playerLight->SetFont(GetOwner()->GetAsset<Base::BaseFont>("main-font"));
  playerLight->SetLayoutSettings({.vAlignment = Base::UIVAlignment::CENTER});
  playerLight->SetFontSize(40);
}
