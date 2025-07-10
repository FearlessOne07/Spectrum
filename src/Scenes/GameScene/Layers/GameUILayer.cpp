#include "GameUILayer.hpp"
#include "Components/HealthComponent.hpp"
#include "Components/LightCollectorComponent.hpp"
#include "Components/Tags/PlayerTag.hpp"
#include "Scenes/GameScene/Signals/GamePause.hpp"
#include "Scenes/GameScene/Signals/GameResume.hpp"
#include "Scenes/MainMenu/MainMenu.hpp"
#include "Signals/EntityDamagedSignal.hpp"
#include "Signals/PlayerSpawnedSignal.hpp"
#include "base/assets/AssetManager.hpp"
#include "base/entities/EntityManager.hpp"
#include "base/input/Events/KeyEvent.hpp"
#include "base/scenes/Scene.hpp"
#include "base/signals/SignalBus.hpp"
#include "base/ui/UIElement.hpp"
#include "base/ui/UILayer.hpp"
#include "base/ui/UILayoutSettings.hpp"
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
  InitBuyMenu();

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
  GetOwner()->GetUIManager()->RemoveLayer("hud");
  GetOwner()->GetUIManager()->RemoveLayer("pause-menu");
  GetOwner()->GetUIManager()->RemoveLayer("buy-menu");
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
      if (_buyMenu->IsVisible())
      {
        _buyMenu->Hide();
        bus->BroadCastSignal(rSig);
      }
      else if (_pauseMenu->IsVisible())
      {
        _pauseMenu->Hide();
        bus->BroadCastSignal(rSig);
      }
      else if (!_pauseMenu->IsVisible())
      {
        bus->BroadCastSignal(pSig);
        _pauseMenu->Show();
      }
      event->isHandled = true;
    }
    else if (keyEvent->key == KEY_TAB && keyEvent->action == Base::InputEvent::Action::PRESSED)
    {
      if (_buyMenu->IsVisible())
      {
        _buyMenu->Hide();
        bus->BroadCastSignal(rSig);
      }
      else if (!_buyMenu->IsVisible() && !_pauseMenu->IsVisible())
      {
        bus->BroadCastSignal(pSig);
        _buyMenu->Show();
      }
      event->isHandled = true;
    }
  }
  GetOwner()->GetUIManager()->OnInputEvent(event);
}

void GameUILayer::Update(float dt)
{
  auto player = GetOwner()->GetEntityManager()->Query<PlayerTag>();
  if (player.size() > 0)
  {
    auto lightcmp = player[0]->item->GetComponent<LightCollectorComponent>();
    auto playerLight = _hud->GetElement<Base::UIContainer>("light-container")->GetChild<Base::UILabel>("player-light");
    playerLight->SetText(TextFormat("%i", lightcmp->value));
  }
}

void GameUILayer::Render()
{
  GetOwner()->GetUIManager()->RenderLayer("hud");
  GetOwner()->GetUIManager()->RenderLayer("buy-menu");
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

  float offset = 100 + (GetSize().y / 2);
  container->_onShow = [this, container, offset]() {
    GetOwner()->GetTweenManager()->AddTween<float>( //
      Base::TweenKey{container.get(), "pause-menu-container-position"},
      [container, this](float pos) { container->SetPositionalOffset({0, pos}); }, 0, offset, 0.5 //
    );
  };
  container->_onHide = [this, container]() {
    GetOwner()->GetTweenManager()->AddTween<float>( //
      Base::TweenKey{container.get(), "pause-menu-container-position"},
      [container, this](float pos) { container->SetPositionalOffset({0, pos}); }, container->GetPositionalOffset().y, 0,
      .3,
      Base::TweenManager::EasingType::EASE_IN //
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
  resumeButton->SetSprite(
    {GetAsset<Base::Texture>("button"), {.top = 4, .bottom = 10, .left = 4, .right = 4}, {13, 2}, {32, 32}} //
  );
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
        {resumeButton.get(), "font-size"},                           //
        [=](float size) { resumeButton->SetFontSize(size, false); }, //
        resumeButton->GetFontSize(),                                 //
        resumeButton->GetBaseFontSize(), 0.1,
        Base::TweenManager::EasingType::EASE_OUT //
      );
    },
  };

  // Exit Button
  auto mainMenuButton = container->AddChild<Base::UIButton>("main-menu-button");
  mainMenuButton->SetFont(GetOwner()->GetAsset<Base::BaseFont>("main-font"));
  mainMenuButton->SetText("Main Menu");
  mainMenuButton->SetFontSize(50);
  mainMenuButton->SetLayoutSettings({.hAlignment = Base::UIHAlignment::CENTER});
  mainMenuButton->onClick = [this]() {
    GetOwner()->SetSceneTransition<MainMenu>(Base::SceneRequest::REPLACE_CURRENT_SCENE);
  };
  mainMenuButton->SetSprite(
    {GetAsset<Base::Texture>("button"), {.top = 4, .bottom = 10, .left = 4, .right = 4}, {13, 2}, {32, 32}} //
  );
  mainMenuButton->onHover = {
    [=, this]() { //
      GetOwner()->GetTweenManager()->AddTween<float>(
        {mainMenuButton.get(), "font-size"}, [=](float size) { mainMenuButton->SetFontSize(size, false); },
        mainMenuButton->GetFontSize(), 55, 0.1,
        Base::TweenManager::EasingType::EASE_OUT //
      );
    },
    [=, this]() { //
      GetOwner()->GetTweenManager()->AddTween<float>(
        {mainMenuButton.get(), "font-size"}, [=](float size) { mainMenuButton->SetFontSize(size, false); },
        mainMenuButton->GetFontSize(), mainMenuButton->GetBaseFontSize(), 0.1,
        Base::TweenManager::EasingType::EASE_OUT //
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

void GameUILayer::InitBuyMenu()
{
  _buyMenu = GetOwner()->GetUIManager()->AddLayer("buy-menu");
  auto mainContainer = _buyMenu->AddElement<Base::UIContainer>("main-container");
  mainContainer->SetElementSizeMode(Base::UIElement::ElementSizeMode::FIXED);
  mainContainer->SetSize(GetSize());
  mainContainer->SetGapMode(Base::UIContainer::GapMode::AUTO);
  mainContainer->SetLayout(Base::UIContainer::Layout::HORIZONTAL);
  mainContainer->SetPadding({200, 300});
  mainContainer->SetPosition({0, -GetSize().y});
  mainContainer->_onShow = [this, mainContainer]() {
    GetOwner()->GetTweenManager()->AddTween<float>( //
      Base::TweenKey{mainContainer.get(), "buy-menu-container-position"},
      [mainContainer, this](float pos) { mainContainer->SetPositionalOffset({0, pos}); }, //
      mainContainer->GetPositionalOffset().y, GetSize().y, 0.5                            //
    );
  };
  mainContainer->_onHide = [this, mainContainer]() {
    GetOwner()->GetTweenManager()->AddTween<float>( //
      Base::TweenKey{mainContainer.get(), "buy-menu-container-position"},
      [mainContainer, this](float pos) { mainContainer->SetPositionalOffset({0, pos}); },
      mainContainer->GetPositionalOffset().y, 0, .3, Base::TweenManager::EasingType::EASE_IN //
    );
  };

  Vector2 cardSize = {350, 500};
  auto card1 = mainContainer->AddChild<Base::UIContainer>("card1");
  card1->SetSize(cardSize);
  card1->SetBackgroundColor(WHITE);
  card1->SetElementSizeMode(Base::UIElement::ElementSizeMode::FIXED);
  card1->SetLayoutSettings({.vAlignment = Base::UIVAlignment::CENTER});
  card1->onHover = {
    [=, this]() {
      GetOwner()->GetTweenManager()->AddTween<float>(
        {card1.get(), "y-pos-offset"},                            //
        [=](float pos) { card1->SetPositionalOffset({0, pos}); }, //
        card1->GetPositionalOffset().y,                           //
        -50, 0.1,
        Base::TweenManager::EasingType::EASE_OUT //
      );
    },
    [=, this]() {
      GetOwner()->GetTweenManager()->AddTween<float>(
        {card1.get(), "y-pos-offset"},                            //
        [=](float pos) { card1->SetPositionalOffset({0, pos}); }, //
        card1->GetPositionalOffset().y,                           //
        0, 0.1,
        Base::TweenManager::EasingType::EASE_OUT //
      );
    },
  };

  auto card2 = mainContainer->AddChild<Base::UIContainer>("card2");
  card2->SetSize(cardSize);
  card2->SetElementSizeMode(Base::UIElement::ElementSizeMode::FIXED);
  card2->SetLayoutSettings({.vAlignment = Base::UIVAlignment::CENTER});
  card2->SetBackgroundColor(WHITE);

  auto card3 = mainContainer->AddChild<Base::UIContainer>("card3");
  card3->SetSize(cardSize);
  card3->SetElementSizeMode(Base::UIElement::ElementSizeMode::FIXED);
  card3->SetLayoutSettings({.vAlignment = Base::UIVAlignment::CENTER});
  card3->SetBackgroundColor(WHITE);
  _buyMenu->Hide();
}
