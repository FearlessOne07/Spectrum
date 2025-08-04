#include "GameUILayer.hpp"
#include "Components/HealthComponent.hpp"
#include "Components/LightCollectorComponent.hpp"
#include "Scenes/GameScene/SharedGameData.hpp"
#include "Scenes/GameScene/Signals/GamePause.hpp"
#include "Scenes/GameScene/Signals/GameResume.hpp"
#include "Scenes/MainMenu/MainMenu.hpp"
#include "base/assets/AssetManager.hpp"
#include "base/entities/EntityManager.hpp"
#include "base/input/Events/KeyEvent.hpp"
#include "base/scenes/Scene.hpp"
#include "base/scenes/SceneLayer.inl"
#include "base/signals/SignalBus.hpp"
#include "base/tween/ITween.hpp"
#include "base/ui/UIElement.hpp"
#include "base/ui/UILayer.hpp"
#include "base/ui/UILayoutSettings.hpp"
#include "base/ui/elements/UIButton.hpp"
#include "base/ui/elements/UIContainer.hpp"
#include "base/ui/elements/UILabel.hpp"
#include "base/ui/elements/UIPanel.hpp"
#include "base/ui/elements/UITextureRect.hpp"
#include "raylib.h"
#include <array>
#include <format>
#include <memory>
#include <string>

void GameUILayer::OnAttach()
{
  // Init UI
  InitHud();
  InitPauseMenu();
  InitShopMenu();

  _shop.Init(this);
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
    if (keyEvent->key == KEY_ESCAPE && keyEvent->action == Base::InputEvent::Action::PRESSED)
    {
      if (_buyMenu->IsVisible())
      {
        _buyMenu->Hide();
        UnPause();
      }
      else if (_pauseMenu->IsVisible())
      {
        _pauseMenu->Hide();
        UnPause();
      }
      else if (!_pauseMenu->IsVisible())
      {
        Pause();
        _pauseMenu->Show();
      }
      event->isHandled = true;
    }
    else if (keyEvent->key == KEY_TAB && keyEvent->action == Base::InputEvent::Action::PRESSED)
    {
      if (_buyMenu->IsVisible())
      {
        CloseShop();
      }
      else if (!_buyMenu->IsVisible() && !_pauseMenu->IsVisible())
      {
        OpenShop();
      }
      event->isHandled = true;
    }
  }
  GetOwner()->GetUIManager()->OnInputEvent(event);
}

void GameUILayer::Update(float dt)
{
  auto player = GetOwner()->GetEntityManager()->GetEntity(GetOwner()->GetSharedData<SharedGameData>()->playerId);

  if (player)
  {
    // Update Light Hud
    auto lightcmp = player->GetComponent<LightCollectorComponent>();
    auto playerLightHud =
      _hud->GetElement<Base::UIContainer>("light-container")->GetChild<Base::UILabel>("player-light");
    auto playerLightBuy =
      _buyMenu->GetElement<Base::UIContainer>("light-container")->GetChild<Base::UILabel>("player-light");
    playerLightHud->SetText(std::format("{0}", lightcmp->value));
    playerLightBuy->SetText(std::format("{0}", lightcmp->value));

    // Updated Health Hud
    auto hlthcmp = player->GetComponent<HealthComponent>();
    auto playerHealth =
      _hud->GetElement<Base::UIContainer>("player-health-container")->GetChild<Base::UILabel>("player-health");
    playerHealth->SetText(std::format("{0}/{1}", hlthcmp->GetHealth(), hlthcmp->GetMaxHealth()));
  }

  auto fps = _hud->GetElement<Base::UIContainer>("fps-container")->GetChild<Base::UILabel>("fps");
  fps->SetText(std::format("FPS:{0}", GetFPS()));
}

void GameUILayer::Render()
{
  GetOwner()->GetUIManager()->RenderLayer("hud");
  GetOwner()->GetUIManager()->RenderLayer("buy-menu");
  GetOwner()->GetUIManager()->RenderLayer("pause-menu");
}

void GameUILayer::InitPauseMenu()
{
  Base::NinePatchSprite buttonSprite = {
    GetAsset<Base::Texture>("button"), {.top = 1, .bottom = 1, .left = 1, .right = 1}, {0, 0}, {16, 8}, 4,
  };

  _pauseMenu = GetOwner()->GetUIManager()->AddLayer("pause-menu");
  auto container = _pauseMenu->AddElement<Base::UIContainer>("pause-menu-container");
  container->SetAnchorPoint(Base::UIContainer::AnchorPoint::CENTER);
  container->SetPosition({GetSize().x / 2, -100});
  container->SetPadding({10, 10});
  container->SetGapSize(15);

  float offset = 100 + (GetSize().y / 2);
  container->onShow = [this, container, offset]() {
    GetOwner()->GetTweenManager()->AddTween<Vector2>( //
      Base::TweenKey{container.get(), "pause-menu-container-position"},
      [container, this](Vector2 pos) { container->SetPositionalOffset({pos}); },
      {
        .startValue = container->GetPositionalOffset(),
        .endValue = {0, offset},
        .duration = 0.5,
      } //
    );
  };

  container->onHide = [this, container]() {
    GetOwner()->GetTweenManager()->AddTween<Vector2>( //
      Base::TweenKey{container.get(), "pause-menu-container-position"},
      [container, this](Vector2 pos) { container->SetPositionalOffset(pos); },
      {.startValue = container->GetPositionalOffset(),
       .endValue = {0, 0},
       .duration = .3,
       .easingType = Base::TweenManager::EasingType::EASE_IN,
       .onTweenEnd = [container]() { container->SetVisibilityOff(); }} //
    );
  };

  // Resume Button
  auto resumeButton = container->AddChild<Base::UIButton>("resume-button");
  resumeButton->SetFont(GetOwner()->GetAsset<Base::BaseFont>("main-font"));
  resumeButton->SetText("Resume");
  resumeButton->SetFontSize(50);
  resumeButton->SetLayoutSettings({.hAlignment = Base::UIHAlignment::CENTER});
  resumeButton->onClick = [this]() {
    _pauseMenu->Hide();
    UnPause();
  };
  resumeButton->SetSprite(buttonSprite);
  resumeButton->onHover = {
    [=, this]() {                                     //
      GetOwner()->GetTweenManager()->AddTween<float>( //
        {resumeButton.get(), "font-size"}, [=](float size) { resumeButton->SetFontSize(size, false); },
        {
          .startValue = resumeButton->GetFontSize(),
          .endValue = 55,
          .duration = 0.1,
          .easingType = Base::TweenManager::EasingType::EASE_OUT,
        } //
      );
    },
    [=, this]() {                                                    //
      GetOwner()->GetTweenManager()->AddTween<float>(                //
        {resumeButton.get(), "font-size"},                           //
        [=](float size) { resumeButton->SetFontSize(size, false); }, //
        {
          .startValue = resumeButton->GetFontSize(), //
          .endValue = resumeButton->GetBaseFontSize(),
          .duration = 0.1,
          .easingType = Base::TweenManager::EasingType::EASE_OUT,
        } //
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
  mainMenuButton->SetSprite(buttonSprite);
  mainMenuButton->onHover = {
    [=, this]() {                                     //
      GetOwner()->GetTweenManager()->AddTween<float>( //
        {mainMenuButton.get(), "font-size"}, [=](float size) { mainMenuButton->SetFontSize(size, false); },
        {
          .startValue = mainMenuButton->GetFontSize(),
          .endValue = 55,
          .duration = 0.1,
          .easingType = Base::TweenManager::EasingType::EASE_OUT,
        } //
      );
    },
    [=, this]() {                                     //
      GetOwner()->GetTweenManager()->AddTween<float>( //
        {mainMenuButton.get(), "font-size"}, [=](float size) { mainMenuButton->SetFontSize(size, false); },
        {
          .startValue = mainMenuButton->GetFontSize(),
          .endValue = mainMenuButton->GetBaseFontSize(),
          .duration = 0.1,
          .easingType = Base::TweenManager::EasingType::EASE_OUT,
        } //
      );
    },
  };

  auto pauseMenuPanel = _pauseMenu->AddElement<Base::UIPanel>("pause-menu-panel");
  pauseMenuPanel->SetPosition({0, 0});
  Color panelColor = {7, 7, 10, 255};
  panelColor = {panelColor.r, panelColor.g, panelColor.b, 0};
  pauseMenuPanel->SetAlpha(0);
  pauseMenuPanel->SetColor(panelColor);
  pauseMenuPanel->SetSize(GetSize());
  pauseMenuPanel->onHide = [=, this]() {
    GetOwner()->GetTweenManager()->AddTween<float>( //
      Base::TweenKey{pauseMenuPanel.get(), "pause-menu-panel-alpha"},
      [this, pauseMenuPanel, panelColor](float alpha) { pauseMenuPanel->SetAlpha(alpha); },
      {
        .startValue = pauseMenuPanel->GetAlpha(),
        .endValue = 0,
        .duration = 0.3,
        .easingType = Base::TweenManager::EasingType::EASE_IN,
        .onTweenEnd = [pauseMenuPanel]() { pauseMenuPanel->SetVisibilityOff(); },
      } //
    );
  };
  pauseMenuPanel->onShow = [=, this]() {
    GetOwner()->GetTweenManager()->AddTween<float>( //
      Base::TweenKey{pauseMenuPanel.get(), "pause-menu-panel-alpha"},
      [this, pauseMenuPanel, panelColor](float alpha) { pauseMenuPanel->SetAlpha(alpha); },
      {
        .startValue = pauseMenuPanel->GetAlpha(),
        .endValue = 0.85,
        .duration = 0.5,
      } //
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
  heartIcon->SetSprite({GetAsset<Base::Texture>("heart-ui"), {}, {2, 0}, {8, 8}});
  heartIcon->SetSize({40, 40});
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
  lightIcon->SetSprite({GetAsset<Base::Texture>("power-ups"), {}, {2, 1}, {8, 8}});
  lightIcon->SetLayoutSettings({.vAlignment = Base::UIVAlignment::CENTER});
  lightIcon->SetSize({40, 40});

  auto playerLight = lightContainer->AddChild<Base::UILabel>("player-light");
  playerLight->SetFont(GetOwner()->GetAsset<Base::BaseFont>("main-font"));
  playerLight->SetLayoutSettings({.vAlignment = Base::UIVAlignment::CENTER});
  playerLight->SetFontSize(40);

  auto fpsContainer = _hud->AddElement<Base::UIContainer>("fps-container");
  fpsContainer->SetLayout(Base::UIContainer::Layout::HORIZONTAL);
  fpsContainer->SetGapSize(20);
  fpsContainer->SetPadding({15, 10});
  fpsContainer->SetPosition({0, 120});

  auto fps = fpsContainer->AddChild<Base::UILabel>("fps");
  fps->SetFont(GetOwner()->GetAsset<Base::BaseFont>("main-font"));
  fps->SetFontSize(40);
  fps->SetLayoutSettings({.vAlignment = Base::UIVAlignment::CENTER});
}

void GameUILayer::InitShopMenu()
{
  Base::NinePatchSprite cardSprite = {
    GetAsset<Base::Texture>("button"), {.top = 1, .bottom = 1, .left = 1, .right = 1}, {0, 0}, {16, 8}, 2,
  };

  _buyMenu = GetOwner()->GetUIManager()->AddLayer("buy-menu");
  float buyMenuEntryDuration = 0.5;
  float buyMenuExitDuration = 0.3;
  auto mainContainer = _buyMenu->AddElement<Base::UIContainer>("shop-menu-container");
  mainContainer->SetElementSizeMode(Base::UIElement::ElementSizeMode::FIXED);
  mainContainer->SetSize(GetSize());
  mainContainer->SetGapMode(Base::UIContainer::GapMode::AUTO);
  mainContainer->SetLayout(Base::UIContainer::Layout::HORIZONTAL);
  mainContainer->SetPadding({200, 300});
  mainContainer->SetPosition({0, -GetSize().y});
  mainContainer->onShow = [this, mainContainer, buyMenuEntryDuration]() {
    GetOwner()->GetTweenManager()->AddTween<Vector2>( //
      Base::TweenKey{mainContainer.get(), "shop-menu-container-position"},
      [mainContainer, this](Vector2 pos) { mainContainer->SetPositionalOffset(pos); }, //
      {
        .startValue = mainContainer->GetPositionalOffset(),
        .endValue = {0, GetSize().y},
        .duration = buyMenuEntryDuration,
      } //
    );
  };
  mainContainer->onHide = [this, mainContainer, buyMenuExitDuration]() {
    GetOwner()->GetTweenManager()->AddTween<Vector2>( //
      Base::TweenKey{mainContainer.get(), "shop-menu-container-position"},
      [mainContainer, this](Vector2 pos) { mainContainer->SetPositionalOffset(pos); },
      {
        .startValue = mainContainer->GetPositionalOffset(),
        .endValue = {0, 0},
        .duration = buyMenuExitDuration,
        .easingType = Base::TweenManager::EasingType::EASE_IN,
        .onTweenEnd = [mainContainer]() { mainContainer->SetVisibilityOff(); },
      } //
    );
  };

  auto lightContainer = _buyMenu->AddElement<Base::UIContainer>("light-container");
  lightContainer->SetLayout(Base::UIContainer::Layout::HORIZONTAL);
  lightContainer->SetGapSize(20);
  lightContainer->SetPadding({15, 10});
  lightContainer->SetPosition({0, 0});
  lightContainer->onShow = [this, lightContainer, buyMenuEntryDuration]() {
    GetOwner()->GetTweenManager()->AddTween<float>(      //
      {lightContainer.get(), "light-container-alpha"},   //
      [=](float pos) { lightContainer->SetAlpha(pos); }, //
      {
        .startValue = lightContainer->GetAlpha(), //
        .endValue = 1,
        .duration = buyMenuEntryDuration,
        .easingType = Base::TweenManager::EasingType::EASE_OUT,
      } //
    );
  };
  lightContainer->onHide = [this, lightContainer, buyMenuExitDuration]() {
    GetOwner()->GetTweenManager()->AddTween<float>(      //
      {lightContainer.get(), "light-container-alpha"},   //
      [=](float pos) { lightContainer->SetAlpha(pos); }, //
      {.startValue = lightContainer->GetAlpha(),         //
       .endValue = 0,
       .duration = buyMenuExitDuration,
       .easingType = Base::TweenManager::EasingType::EASE_IN,
       .onTweenEnd = [lightContainer]() { lightContainer->SetVisibilityOff(); }} //
    );
  };

  auto lightIcon = lightContainer->AddChild<Base::UITextureRect>("light-icon");
  lightIcon->SetSprite({GetAsset<Base::Texture>("power-ups"), {}, {2, 1}, {8, 8}});
  lightIcon->SetLayoutSettings({.vAlignment = Base::UIVAlignment::CENTER});
  lightIcon->SetSize({40, 40});

  auto playerLight = lightContainer->AddChild<Base::UILabel>("player-light");
  playerLight->SetFont(GetOwner()->GetAsset<Base::BaseFont>("main-font"));
  playerLight->SetLayoutSettings({.vAlignment = Base::UIVAlignment::CENTER});
  playerLight->SetFontSize(40);

  Vector2 cardSize = {350, 500};
  float fadeOutDuration = 0.5;
  float fadeInDuration = 0.5;
  for (int i = 0; i < 3; i++)
  {
    auto card = mainContainer->AddChild<Base::UIContainer>(std::format("card{0}", i));
    card->SetSize(cardSize);
    card->SetBackgroundColor(WHITE);
    card->SetGapMode(Base::UIContainer::GapMode::AUTO);
    card->SetElementSizeMode(Base::UIElement::ElementSizeMode::FIXED);
    card->SetLayoutSettings({.vAlignment = Base::UIVAlignment::CENTER});
    card->SetLayout(Base::UIContainer::Layout::VERTICAL);
    card->SetSprite(cardSprite);
    card->SetPadding({0, 100});
    card->onHover = {
      [=, this]() {
        GetOwner()->GetTweenManager()->AddTween<Vector2>(       //
          {card.get(), std::format("y-pos-offset-{0}", i)},     //
          [=](Vector2 pos) { card->SetPositionalOffset(pos); }, //
          {
            .startValue = card->GetPositionalOffset(),
            .endValue = {0, -30},
            .duration = 0.1,
            .easingType = Base::TweenManager::EasingType::EASE_OUT,
          } //
        );
      },
      [=, this]() {
        GetOwner()->GetTweenManager()->AddTween<Vector2>(       //
          {card.get(), std::format("y-pos-offset-{0}", i)},     //
          [=](Vector2 pos) { card->SetPositionalOffset(pos); }, //
          {
            .startValue = card->GetPositionalOffset(),
            .endValue = {0, 0},
            .duration = 0.1,
            .easingType = Base::TweenManager::EasingType::EASE_OUT,
          } //
        );
      },
    };
    card->onClick = [this, i, card, fadeOutDuration, fadeInDuration]() {
      if (BuyItem(i))
      {
        GetOwner()->GetTweenManager()->AddTween<Vector2>(       //
          {card.get(), std::format("y-pos-offset-{0}", i)},     //
          [=](Vector2 pos) { card->SetPositionalOffset(pos); }, //
          {
            .startValue = card->GetPositionalOffset(), //
            .endValue = {0, card->GetPositionalOffset().y - 50},
            .duration = fadeOutDuration,
            .easingType = Base::TweenManager::EasingType::EASE_OUT,
            .priority = Base::TweenPriorityLevel::MEDIUM,
          } //
        );
        GetOwner()->GetTweenManager()->AddTween<float>( //
          {card.get(), std::format("alpha-{0}", i)},    //
          [=](float pos) { card->SetAlpha(pos); },      //
          {
            .startValue = card->GetAlpha(), //
            .endValue = 0,
            .duration = fadeOutDuration,
            .easingType = Base::TweenManager::EasingType::EASE_OUT,
            .onTweenEnd =
              [=, this]() {
                // Refresh Shop
                auto alphas = UpdateItems();

                // Tween Card back to position
                GetOwner()->GetTweenManager()->AddTween<Vector2>(       //
                  {card.get(), std::format("y-pos-offset-{0}", i)},     //
                  [=](Vector2 pos) { card->SetPositionalOffset(pos); }, //
                  {
                    .startValue = card->GetPositionalOffset(), //
                    .endValue = {0, 0},
                    .duration = fadeInDuration,
                    .easingType = Base::TweenManager::EasingType::EASE_OUT,
                    .priority = Base::TweenPriorityLevel::MEDIUM,
                  } //
                );
                GetOwner()->GetTweenManager()->AddTween<float>( //
                  {card.get(), std::format("alpha-{0}", i)},    //
                  [=](float pos) { card->SetAlpha(pos); },      //
                  {
                    .startValue = card->GetAlpha(), //
                    .endValue = alphas[i],
                    .duration = fadeInDuration,
                    .easingType = Base::TweenManager::EasingType::EASE_OUT,
                    .priority = Base::TweenPriorityLevel::MEDIUM,
                  } //
                );
              },
            .priority = Base::TweenPriorityLevel::MEDIUM,
          } //
        );
      };
    };

    auto name = card->AddChild<Base::UILabel>("name");
    name->SetFont(GetAsset<Base::BaseFont>("main-font"));
    name->SetFontSize(30);
    name->SetText("Max Health");
    name->SetLayoutSettings({.hAlignment = Base::UIHAlignment::CENTER});
    name->SetTextColor(BLACK);

    auto icon = card->AddChild<Base::UITextureRect>("icon");
    icon->SetSprite({GetAsset<Base::Texture>("heart-ui"), {}, {2, 0}, {8, 8}});
    icon->SetSize({128, 128});
    icon->SetLayoutSettings({.hAlignment = Base::UIHAlignment::CENTER});

    auto price = card->AddChild<Base::UIContainer>("price-container");
    price->SetLayoutSettings({.hAlignment = Base::UIHAlignment::CENTER});
    price->SetLayout(Base::UIContainer::Layout::HORIZONTAL);
    price->SetGapSize(20);
    price->SetPadding({15, 10});
    price->SetPosition({0, 60});

    auto lightIcon = price->AddChild<Base::UITextureRect>("light-icon");
    lightIcon->SetSprite({GetAsset<Base::Texture>("power-ups"), {}, {2, 1}, {8, 8}});
    lightIcon->SetLayoutSettings({.vAlignment = Base::UIVAlignment::CENTER});
    lightIcon->SetSize({32, 32});

    auto lightCost = price->AddChild<Base::UILabel>("light-cost");
    lightCost->SetFont(GetOwner()->GetAsset<Base::BaseFont>("main-font"));
    lightCost->SetLayoutSettings({.vAlignment = Base::UIVAlignment::CENTER});
    lightCost->SetFontSize(30);
    lightCost->SetTextColor(BLACK);
  }

  auto buyMenuPanel = _buyMenu->AddElement<Base::UIPanel>("buy-menu-panel");
  buyMenuPanel->SetSize(GetSize());
  buyMenuPanel->SetColor(GetOwner()->GetClearColor());
  buyMenuPanel->SetAlpha(0);
  buyMenuPanel->SetPosition({0, 0});
  buyMenuPanel->onShow = [this, buyMenuPanel, buyMenuEntryDuration]() {
    GetOwner()->GetTweenManager()->AddTween<float>(    //
      {buyMenuPanel.get(), "buy-menu-alpha"},          //
      [=](float pos) { buyMenuPanel->SetAlpha(pos); }, //
      {
        .startValue = buyMenuPanel->GetAlpha(), //
        .endValue = 1,
        .duration = buyMenuEntryDuration,
        .easingType = Base::TweenManager::EasingType::EASE_OUT,
      } //
    );
  };
  buyMenuPanel->onHide = [this, buyMenuPanel, buyMenuExitDuration]() {
    GetOwner()->GetTweenManager()->AddTween<float>(    //
      {buyMenuPanel.get(), "buy-menu-alpha"},          //
      [=](float pos) { buyMenuPanel->SetAlpha(pos); }, //
      {.startValue = buyMenuPanel->GetAlpha(),         //
       .endValue = 0,
       .duration = buyMenuExitDuration,
       .easingType = Base::TweenManager::EasingType::EASE_IN,
       .onTweenEnd = [buyMenuPanel]() { buyMenuPanel->SetVisibilityOff(); }} //
    );
  };
  _buyMenu->Hide();
}

bool GameUILayer::BuyItem(int index)
{
  ShopItem item = _shop.GetItem(index);
  auto player = GetOwner()->GetEntityManager()->GetEntity(GetOwner()->GetSharedData<SharedGameData>()->playerId);
  auto lightComp = player->GetComponent<LightCollectorComponent>();

  if (lightComp->value >= item.cost)
  {
    _shop.BuyItem(index);
    item.modifier->Apply(player);
    lightComp->value -= item.cost;
    return true;
  }
  return false;
}

void GameUILayer::OpenShop()
{
  auto bus = Base::SignalBus::GetInstance();
  auto pSig = std::make_shared<GamePausedSignal>();

  UpdateItems();
  bus->BroadCastSignal(pSig);
  _buyMenu->Show();
}

std::array<float, 3> GameUILayer::UpdateItems()
{
  auto &currentItems = _shop.GetItems();
  if (_shop.HasNewItems())
  {
    for (int i = 0; i < currentItems.size(); i++)
    {
      auto card = _buyMenu->GetElement<Base::UIContainer>("shop-menu-container")
                    ->GetChild<Base::UIContainer>(std::format("card{0}", i));
      auto cost = card->GetChild<Base::UIContainer>("price-container")->GetChild<Base::UILabel>("light-cost");
      auto icon = card->GetChild<Base::UITextureRect>("icon");
      auto name = card->GetChild<Base::UILabel>("name");

      cost->SetText(std::to_string(currentItems[i].cost));
      icon->SetSprite(currentItems[i].textureIcon);
      name->SetText(currentItems[i].name);
    }
    _shop.ResetNewItems();
  }

  auto player = GetOwner()->GetEntityManager()->GetEntity(GetOwner()->GetSharedData<SharedGameData>()->playerId);
  std::array<float, 3> alphas;
  float alpha = 0;
  for (int i = 0; i < currentItems.size(); i++)
  {
    auto card = _buyMenu->GetElement<Base::UIContainer>("shop-menu-container")
                  ->GetChild<Base::UIContainer>(std::format("card{0}", i));
    if (currentItems[i].cost > player->GetComponent<LightCollectorComponent>()->value)
    {
      alpha = 0.3;
    }
    else
    {
      alpha = 1;
    }
    alphas[i] = alpha;
    GetOwner()->GetTweenManager()->AddTween<float>( //
      {card.get(), std::format("alpha-{0}", i)},    //
      [=](float pos) { card->SetAlpha(pos); },      //
      {
        .startValue = card->GetAlpha(), //
        .endValue = alpha,
        .duration = 0.3,
        .easingType = Base::TweenManager::EasingType::EASE_OUT,
      } //
    );
  }
  return alphas;
}

void GameUILayer::CloseShop()
{
  _buyMenu->Hide();
  UnPause();
};

void GameUILayer::Pause()
{
  auto bus = Base::SignalBus::GetInstance();
  auto pSig = std::make_shared<GamePausedSignal>();
  bus->BroadCastSignal(pSig);
}

void GameUILayer::UnPause()
{
  auto bus = Base::SignalBus::GetInstance();
  auto rSig = std::make_shared<GameResumedSignal>();
  bus->BroadCastSignal(rSig);
}
