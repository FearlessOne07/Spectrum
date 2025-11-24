#include "GameUILayer.hpp"
#include "Components/HealthComponent.hpp"
#include "Components/LightCollectorComponent.hpp"
#include "Scenes/GameScene/SharedGameData.hpp"
#include "Scenes/GameScene/Signals/GamePause.hpp"
#include "Scenes/GameScene/Signals/GameResume.hpp"
#include "Scenes/MainMenu/MainMenu.hpp"
#include "Signals/PlayerSpawnedSignal.hpp"
#include "base/input/Keys.hpp"
#include "raylib.h"
#include <array>
#include <base/assets/AssetManager.hpp>
#include <base/audio/signals/StopAudioStreamSignal.hpp>
#include <base/entities/Entity.hpp>
#include <base/entities/EntityManager.hpp>
#include <base/input/Events/KeyEvent.hpp>
#include <base/scenes/Scene.hpp>
#include <base/scenes/SceneLayer.tpp>
#include <base/signals/SignalBus.hpp>
#include <base/tween/Tween.hpp>
#include <base/ui/UIElement.hpp>
#include <base/ui/UILayer.hpp>
#include <base/ui/elements/UIButton.hpp>
#include <base/ui/elements/UIFlexContainer.hpp>
#include <base/ui/elements/UILabel.hpp>
#include <base/ui/elements/UIStackPanel.hpp>
#include <base/ui/elements/UITextureRect.hpp>
#include <format>
#include <memory>
#include <string>

void GameUILayer::OnAttach()
{
  // Init UI
  InitPauseMenu();

  _shop.Init(this);

  auto bus = Base::SignalBus::GetInstance();
  bus->SubscribeSignal<PlayerSpawnedSignal>([this](std::shared_ptr<Base::Signal> sig) {
    auto pSpawn = std::static_pointer_cast<PlayerSpawnedSignal>(sig);
    InitHud(pSpawn->player);
    InitShopMenu(pSpawn->player);
  });
}

void GameUILayer::OnDetach()
{
}

void GameUILayer::OnInputEvent(std::shared_ptr<Base::InputEvent> &event)
{
  if (auto keyEvent = std::dynamic_pointer_cast<Base::KeyEvent>(event))
  {
    if (keyEvent->Key == Base::Key::Escape && keyEvent->action == Base::InputEvent::Action::Pressed)
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
    else if (keyEvent->Key == Base::Key::Tab && keyEvent->action == Base::InputEvent::Action::Pressed)
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

  _pauseMenu = GetOwner()->GetUIManager()->AddLayer("pause-menu", GetSize(), {0, 0}, *this);
  auto container = _pauseMenu->SetRootElement<Base::UIStackPanel>();
  container->SetVAlignment(Base::VAlign::Center);
  container->SetHAlignment(Base::HAlign::Center);
  container->SetPadding(10, 10);
  container->SetGap(15);

  float offset = -(100 + (GetSize().y / 2));
  container->GetRenderTransform().SetOffsetY(offset);
  container->onShow = [this, container, offset]() {
    GetOwner()->GetTweenManager()->AddTween<float>( //
      Base::TweenKey{container.get(), "pause-menu-container-y-offset"},
      [container, this](float pos) { container->GetRenderTransform().SetOffsetY(pos); },
      {
        .startValue = container->GetRenderTransform().GetOffsetY(),
        .endValue = 0,
        .duration = 0.5,
      } //
    );
  };

  container->onHide = [this, container, offset]() {
    GetOwner()->GetTweenManager()->AddTween<float>( //
      Base::TweenKey{container.get(), "pause-menu-container-y-offset"},
      [container, this](float pos) { container->GetRenderTransform().SetOffsetY(pos); },
      {
        .startValue = container->GetRenderTransform().GetOffsetY(),
        .endValue = offset,
        .duration = 0.3,
        .easingType = Base::TweenManager::EasingType::EaseIn,
        .onTweenEnd = [container]() { container->SetVisibilityOff(); },
      } //
    );
  };

  // Resume Button
  float hoverScale = 1.1;
  auto resumeButton = container->AddChild<Base::UIButton>("resume-button");
  resumeButton->SetFont(GetAsset<Base::BaseFont>("main-font"));
  resumeButton->SetText("Resume");
  resumeButton->SetFontSize(50);
  resumeButton->SetVAlignment(Base::VAlign::Center);
  resumeButton->SetHAlignment(Base::HAlign::Center);
  resumeButton->SetPadding(10);
  resumeButton->onClick = [this]() {
    _pauseMenu->Hide();
    UnPause();
  };
  resumeButton->SetSprite(buttonSprite);
  resumeButton->onHover = {
    [=, this]() {                                     //
      GetOwner()->GetTweenManager()->AddTween<float>( //
        {resumeButton.get(), "font-size"}, [=](float size) { resumeButton->GetRenderTransform().SetFontScale(size); },
        {
          .startValue = resumeButton->GetRenderTransform().GetFontScale(),
          .endValue = hoverScale,
          .duration = 0.1,
          .easingType = Base::TweenManager::EasingType::EaseOut,
        } //
      );
    },
    [=, this]() {                                                                   //
      GetOwner()->GetTweenManager()->AddTween<float>(                               //
        {resumeButton.get(), "font-size"},                                          //
        [=](float size) { resumeButton->GetRenderTransform().SetFontScale(size); }, //
        {
          .startValue = resumeButton->GetRenderTransform().GetFontScale(), //
          .endValue = 1,
          .duration = 0.1,
          .easingType = Base::TweenManager::EasingType::EaseOut,
        } //
      );
    },
  };

  // Exit Button
  auto mainMenuButton = container->AddChild<Base::UIButton>("main-menu-button");
  mainMenuButton->SetFont(GetOwner()->GetAsset<Base::BaseFont>("main-font"));
  mainMenuButton->SetText("Main Menu");
  mainMenuButton->SetFontSize(50);
  mainMenuButton->SetPadding(10);
  mainMenuButton->SetVAlignment(Base::VAlign::Center);
  mainMenuButton->SetHAlignment(Base::HAlign::Center);
  mainMenuButton->onClick = [this]() {
    auto bus = Base::SignalBus::GetInstance();
    std::shared_ptr<Base::StopAudioStreamSignal> sig = std::make_shared<Base::StopAudioStreamSignal>();
    sig->streamHandle = GetAsset<Base::AudioStream>("game-track");
    bus->BroadCastSignal(sig);
    GetOwner()->SetSceneTransition<MainMenu>(Base::SceneRequest::ReplaceCurrentScene);
  };
  mainMenuButton->SetSprite(buttonSprite);
  mainMenuButton->onHover = {
    [=, this]() {                                     //
      GetOwner()->GetTweenManager()->AddTween<float>( //
        {mainMenuButton.get(), "font-size"},
        [=](float size) { mainMenuButton->GetRenderTransform().SetFontScale(size); },
        {
          .startValue = mainMenuButton->GetRenderTransform().GetFontScale(),
          .endValue = hoverScale,
          .duration = 0.1,
          .easingType = Base::TweenManager::EasingType::EaseOut,
        } //
      );
    },
    [=, this]() {                                     //
      GetOwner()->GetTweenManager()->AddTween<float>( //
        {mainMenuButton.get(), "font-size"},
        [=](float size) { mainMenuButton->GetRenderTransform().SetFontScale(size); },
        {
          .startValue = mainMenuButton->GetRenderTransform().GetFontScale(),
          .endValue = 1,
          .duration = 0.1,
          .easingType = Base::TweenManager::EasingType::EaseOut,
        } //
      );
    },
  };

  auto pauseMenuPanel = _pauseMenu->SetLayerBackPanel();
  Color panelColor = {7, 7, 10, 255};
  panelColor = {panelColor.r, panelColor.g, panelColor.b, 0};
  pauseMenuPanel->GetRenderTransform().SetOpacity(0);
  pauseMenuPanel->SetColor(panelColor);
  pauseMenuPanel->SetSize({GetSize().x, GetSize().y});
  pauseMenuPanel->onHide = [=, this]() {
    GetOwner()->GetTweenManager()->AddTween<float>( //
      Base::TweenKey{pauseMenuPanel.get(), "pause-menu-panel-alpha"},
      [this, pauseMenuPanel, panelColor](float alpha) { pauseMenuPanel->GetRenderTransform().SetOpacity(alpha); },
      {
        .startValue = pauseMenuPanel->GetRenderTransform().GetOpacity(),
        .endValue = 0,
        .duration = 0.3,
        .easingType = Base::TweenManager::EasingType::EaseIn,
        .onTweenEnd = [pauseMenuPanel]() { pauseMenuPanel->SetVisibilityOff(); },
      } //
    );
  };
  pauseMenuPanel->onShow = [=, this]() {
    GetOwner()->GetTweenManager()->AddTween<float>( //
      Base::TweenKey{pauseMenuPanel.get(), "pause-menu-panel-alpha"},
      [this, pauseMenuPanel, panelColor](float alpha) { pauseMenuPanel->GetRenderTransform().SetOpacity(alpha); },
      {
        .startValue = pauseMenuPanel->GetRenderTransform().GetOpacity(),
        .endValue = 0.85,
        .duration = 0.5,
      } //
    );
  };
  _pauseMenu->Hide();
}

void GameUILayer::InitHud(std::shared_ptr<Base::Entity> player)
{
  _hud = GetOwner()->GetUIManager()->AddLayer("hud", GetSize(), {0, 0}, *this);
  auto hudStack = _hud->SetRootElement<Base::UIStackPanel>();
  hudStack->SetOrientation(Base::UIStackPanel::Orientation::Vertical);

  auto healtContainer = hudStack->AddChild<Base::UIStackPanel>("health-stack");
  healtContainer->SetOrientation(Base::UIStackPanel::Orientation::Horizontal);
  healtContainer->SetGap(20);
  healtContainer->SetPadding(15, 10);

  auto heartIcon = healtContainer->AddChild<Base::UITextureRect>("heart-icon");
  heartIcon->SetSprite({GetAsset<Base::Texture>("heart-ui"), {}, {16, 0}, {8, 8}});
  heartIcon->SetSize({40, 40});
  heartIcon->SetVAlignment(Base::VAlign::Center);
  heartIcon->SetHAlignment(Base::HAlign::Center);

  auto hlthcmp = player->GetComponent<HealthComponent>();
  auto playerHealth = healtContainer->AddChild<Base::UILabel>("player-health");
  playerHealth->SetFont(GetOwner()->GetAsset<Base::BaseFont>("main-font"));
  playerHealth->SetFontSize(40);
  playerHealth->SetVAlignment(Base::VAlign::Center);
  playerHealth->SetHAlignment(Base::HAlign::Center);
  playerHealth->Bind({
    [hlthcmp]() -> std::string { return std::format("{:.0f}/{:.0f}", hlthcmp->GetHealth(), hlthcmp->GetMaxHealth()); },
  });

  auto lightContainer = hudStack->AddChild<Base::UIStackPanel>("light-stack");
  lightContainer->SetOrientation(Base::UIStackPanel::Orientation::Horizontal);
  lightContainer->SetGap(20);
  lightContainer->SetPadding(15, 10);

  auto lightIcon = lightContainer->AddChild<Base::UITextureRect>("light-icon");
  lightIcon->SetSprite({GetAsset<Base::Texture>("power-ups"), {}, {16, 8}, {8, 8}});
  lightIcon->SetVAlignment(Base::VAlign::Center);
  lightIcon->SetHAlignment(Base::HAlign::Center);
  lightIcon->SetSize({40, 40});

  auto lightcmp = player->GetComponent<LightCollectorComponent>();
  auto playerLight = lightContainer->AddChild<Base::UILabel>("player-light");
  playerLight->SetFont(GetOwner()->GetAsset<Base::BaseFont>("main-font"));
  playerLight->SetVAlignment(Base::VAlign::Center);
  playerLight->SetHAlignment(Base::HAlign::Center);
  playerLight->SetFontSize(40);
  playerLight->Bind({[lightcmp]() -> std::string { return std::format("{}", lightcmp->value); }});

  auto fpsContainer = hudStack->AddChild<Base::UIStackPanel>("fps-stack");
  fpsContainer->SetOrientation(Base::UIStackPanel::Orientation::Horizontal);
  fpsContainer->SetGap(20);
  fpsContainer->SetPadding(15, 10);

  auto fps = fpsContainer->AddChild<Base::UILabel>("fps");
  fps->SetFont(GetOwner()->GetAsset<Base::BaseFont>("main-font"));
  fps->SetFontSize(40);
  fps->SetVAlignment(Base::VAlign::Center);
  fps->SetHAlignment(Base::HAlign::Center);
  fps->Bind({[]() { return std::format("FPS:{}", GetFPS()); }});
}

void GameUILayer::InitShopMenu(std::shared_ptr<Base::Entity> player)
{
  float buyMenuEntryDuration = 0.5;
  float buyMenuExitDuration = 0.3;
  Base::NinePatchSprite cardSprite = {
    GetAsset<Base::Texture>("button"), {.top = 1, .bottom = 1, .left = 1, .right = 1}, {0, 0}, {16, 8}, 8,
  };

  _buyMenu = GetOwner()->GetUIManager()->AddLayer("buy-menu", GetSize(), {0, 0}, *this);
  auto buyMenuPanel = _buyMenu->SetLayerBackPanel();

  buyMenuPanel->SetSize({GetSize().x, GetSize().y});
  buyMenuPanel->SetColor(GetOwner()->GetClearColor());
  buyMenuPanel->GetRenderTransform().SetOpacity(0);
  buyMenuPanel->onShow = [this, buyMenuPanel, buyMenuEntryDuration]() {
    GetOwner()->GetTweenManager()->AddTween<float>(                           //
      {buyMenuPanel.get(), "buy-menu-alpha"},                                 //
      [=](float pos) { buyMenuPanel->GetRenderTransform().SetOpacity(pos); }, //
      {
        .startValue = buyMenuPanel->GetRenderTransform().GetOpacity(), //
        .endValue = 1,
        .duration = buyMenuEntryDuration,
        .easingType = Base::TweenManager::EasingType::EaseOut,
      } //
    );
  };

  buyMenuPanel->onHide = [this, buyMenuPanel, buyMenuExitDuration]() {
    GetOwner()->GetTweenManager()->AddTween<float>(                           //
      {buyMenuPanel.get(), "buy-menu-alpha"},                                 //
      [=](float pos) { buyMenuPanel->GetRenderTransform().SetOpacity(pos); }, //
      {.startValue = buyMenuPanel->GetRenderTransform().GetOpacity(),         //
       .endValue = 0,
       .duration = buyMenuExitDuration,
       .easingType = Base::TweenManager::EasingType::EaseIn,
       .onTweenEnd = [buyMenuPanel]() { buyMenuPanel->SetVisibilityOff(); }} //
    );
  };

  float offset = -GetSize().y;
  auto mainContainer = _buyMenu->SetRootElement<Base::UIStackPanel>();
  mainContainer->SetVAlignment(Base::VAlign::Stretch);
  mainContainer->SetHAlignment(Base::HAlign::Stretch);
  mainContainer->SetOrientation(Base::UIStackPanel::Orientation::Vertical);
  mainContainer->GetRenderTransform().SetOffsetY(offset);
  mainContainer->SetGap(150);
  mainContainer->onShow = [this, mainContainer, buyMenuEntryDuration]() {
    GetOwner()->GetTweenManager()->AddTween<float>( //
      Base::TweenKey{mainContainer.get(), "shop-menu-container-position"},
      [mainContainer, this](float pos) { mainContainer->GetRenderTransform().SetOffsetY(pos); }, //
      {
        .startValue = mainContainer->GetRenderTransform().GetOffsetY(),
        .endValue = 0,
        .duration = buyMenuEntryDuration,
      } //
    );
  };
  mainContainer->onHide = [this, mainContainer, buyMenuExitDuration, offset]() {
    GetOwner()->GetTweenManager()->AddTween<float>( //
      Base::TweenKey{mainContainer.get(), "shop-menu-container-position"},
      [mainContainer, this](float pos) { mainContainer->GetRenderTransform().SetOffsetY(pos); },
      {
        .startValue = mainContainer->GetRenderTransform().GetOffsetY(),
        .endValue = offset,
        .duration = buyMenuExitDuration,
        .easingType = Base::TweenManager::EasingType::EaseIn,
        .onTweenEnd = [mainContainer]() { mainContainer->SetVisibilityOff(); },
      } //
    );
  };

  auto lightContainer = mainContainer->AddChild<Base::UIStackPanel>("light-container");
  lightContainer->SetOrientation(Base::UIStackPanel::Orientation::Horizontal);
  lightContainer->SetGap(20);
  lightContainer->SetPadding(15, 10);
  lightContainer->SetVAlignment(Base::VAlign::Center);
  lightContainer->SetHAlignment(Base::HAlign::Left);
  lightContainer->onShow = [this, lightContainer, buyMenuEntryDuration]() {
    GetOwner()->GetTweenManager()->AddTween<float>(                             //
      {lightContainer.get(), "light-container-alpha"},                          //
      [=](float pos) { lightContainer->GetRenderTransform().SetOpacity(pos); }, //
      {
        .startValue = lightContainer->GetRenderTransform().GetOpacity(), //
        .endValue = 1,
        .duration = buyMenuEntryDuration,
        .easingType = Base::TweenManager::EasingType::EaseOut,
      } //
    );
  };
  lightContainer->onHide = [this, lightContainer, buyMenuExitDuration]() {
    GetOwner()->GetTweenManager()->AddTween<float>(                             //
      {lightContainer.get(), "light-container-alpha"},                          //
      [=](float pos) { lightContainer->GetRenderTransform().SetOpacity(pos); }, //
      {
        .startValue = lightContainer->GetRenderTransform().GetOpacity(), //
        .endValue = 0,
        .duration = buyMenuExitDuration,
        .easingType = Base::TweenManager::EasingType::EaseIn,
        .onTweenEnd = [lightContainer]() { lightContainer->SetVisibilityOff(); },
      } //
    );
  };

  auto lightIcon = lightContainer->AddChild<Base::UITextureRect>("light-icon");
  lightIcon->SetSprite({GetAsset<Base::Texture>("power-ups"), {}, {16, 8}, {8, 8}});
  lightIcon->SetVAlignment(Base::VAlign::Center);
  lightIcon->SetHAlignment(Base::HAlign::Center);
  lightIcon->SetSize({40, 40});

  auto lightcmp = player->GetComponent<LightCollectorComponent>();
  auto playerLight = lightContainer->AddChild<Base::UILabel>("player-light");
  playerLight->SetFont(GetOwner()->GetAsset<Base::BaseFont>("main-font"));
  playerLight->SetVAlignment(Base::VAlign::Center);
  playerLight->SetHAlignment(Base::HAlign::Center);
  playerLight->SetFontSize(40);
  playerLight->Bind({[lightcmp]() -> std::string { return std::format("{}", lightcmp->value); }});

  auto cardStack = mainContainer->AddChild<Base::UIFlexContainer>("card-stack");
  cardStack->SetOrientation(Base::UIFlexContainer::Orientation::Horizontal);
  cardStack->SetPadding(100, 100);
  cardStack->SetHAlignment(Base::HAlign::Stretch);

  Base::Size cardSize = {350, 500};
  float fadeOutDuration = 0.5;
  float fadeInDuration = 0.5;

  for (int i = 0; i < 3; i++)
  {
    auto card = cardStack->AddChild<Base::UIStackPanel>(std::format("card{0}", i));
    card->SetWidth(cardSize.width);
    card->SetSizeMode(Base::SizeMode::Fixed);
    card->SetSize(cardSize);

    card->SetHAlignment(Base::HAlign::Center);
    card->SetVAlignment(Base::VAlign::Center);
    card->SetSprite(cardSprite);
    card->SetPadding(100, 100);
    card->SetGap(60);
    card->onHover = {
      [=, this]() {
        GetOwner()->GetTweenManager()->AddTween<float>(                   //
          {card.get(), std::format("y-pos-offset-{0}", i)},               //
          [=](float pos) { card->GetRenderTransform().SetOffsetY(pos); }, //
          {
            .startValue = card->GetRenderTransform().GetOffsetY(),
            .endValue = -30,
            .duration = 0.1,
            .easingType = Base::TweenManager::EasingType::EaseOut,
          } //
        );
      },
      [=, this]() {
        GetOwner()->GetTweenManager()->AddTween<float>(                   //
          {card.get(), std::format("y-pos-offset-{0}", i)},               //
          [=](float pos) { card->GetRenderTransform().SetOffsetY(pos); }, //
          {
            .startValue = card->GetRenderTransform().GetOffsetY(),
            .endValue = 0,
            .duration = 0.1,
            .easingType = Base::TweenManager::EasingType::EaseOut,
          } //
        );
      },
    };
    card->onClick = [this, i, card, fadeOutDuration, fadeInDuration]() {
      if (BuyItem(i))
      {
        GetOwner()->GetTweenManager()->AddTween<float>(                   //
          {card.get(), std::format("y-pos-offset-{0}", i)},               //
          [=](float pos) { card->GetRenderTransform().SetOffsetY(pos); }, //
          {
            .startValue = card->GetRenderTransform().GetOffsetY(), //
            .endValue = card->GetRenderTransform().GetOffsetY() - 50,
            .duration = fadeOutDuration,
            .easingType = Base::TweenManager::EasingType::EaseOut,
            .priority = Base::TweenPriorityLevel::Medium,
          } //
        );

        GetOwner()->GetTweenManager()->AddTween<float>(                   //
          {card.get(), std::format("alpha-{0}", i)},                      //
          [=](float pos) { card->GetRenderTransform().SetOpacity(pos); }, //
          {
            .startValue = card->GetRenderTransform().GetOpacity(), //
            .endValue = 0,
            .duration = fadeOutDuration,
            .easingType = Base::TweenManager::EasingType::EaseOut,
            .onTweenEnd =
              [=, this]() {
                // Refresh Shop
                auto alphas = UpdateItems();

                // Tween Card back to position
                GetOwner()->GetTweenManager()->AddTween<float>(                   //
                  {card.get(), std::format("y-pos-offset-{0}", i)},               //
                  [=](float pos) { card->GetRenderTransform().SetOffsetY(pos); }, //
                  {
                    .startValue = card->GetRenderTransform().GetOffsetY(), //
                    .endValue = 0,
                    .duration = fadeInDuration,
                    .easingType = Base::TweenManager::EasingType::EaseOut,
                    .priority = Base::TweenPriorityLevel::Medium,
                  } //
                );

                // Tween Cards to the correct alpha
                GetOwner()->GetTweenManager()->AddTween<float>(                   //
                  {card.get(), std::format("alpha-{0}", i)},                      //
                  [=](float pos) { card->GetRenderTransform().SetOpacity(pos); }, //
                  {
                    .startValue = card->GetRenderTransform().GetOpacity(), //
                    .endValue = alphas[i],
                    .duration = fadeInDuration,
                    .easingType = Base::TweenManager::EasingType::EaseOut,
                    .priority = Base::TweenPriorityLevel::Medium,
                  } //
                );
              },
            .priority = Base::TweenPriorityLevel::Medium,
          } //
        );
      };
    };

    auto name = card->AddChild<Base::UILabel>("name");
    name->SetFont(GetAsset<Base::BaseFont>("main-font"));
    name->SetFontSize(30);
    name->SetText("Max Health");
    name->SetVAlignment(Base::VAlign::Center);
    name->SetHAlignment(Base::HAlign::Center);
    name->SetTextColor(BLACK);

    auto icon = card->AddChild<Base::UITextureRect>("icon");
    icon->SetSprite({GetAsset<Base::Texture>("heart-ui"), {}, {16, 0}, {8, 8}});
    icon->SetSize({128, 128});
    icon->SetVAlignment(Base::VAlign::Center);
    icon->SetHAlignment(Base::HAlign::Center);

    auto price = card->AddChild<Base::UIStackPanel>("price-container");
    price->SetVAlignment(Base::VAlign::Center);
    price->SetHAlignment(Base::HAlign::Center);
    price->SetOrientation(Base::UIStackPanel::Orientation::Horizontal);
    price->SetGap(20);
    price->SetPadding(15, 10);

    auto lightIcon = price->AddChild<Base::UITextureRect>("light-icon");
    lightIcon->SetSprite({GetAsset<Base::Texture>("power-ups"), {}, {16, 8}, {8, 8}});
    lightIcon->SetVAlignment(Base::VAlign::Center);
    lightIcon->SetHAlignment(Base::HAlign::Center);
    lightIcon->SetSize({32, 32});

    auto lightCost = price->AddChild<Base::UILabel>("light-cost");
    lightCost->SetFont(GetOwner()->GetAsset<Base::BaseFont>("main-font"));
    lightCost->SetVAlignment(Base::VAlign::Center);
    lightCost->SetHAlignment(Base::HAlign::Center);
    lightCost->SetFontSize(30);
    lightCost->SetTextColor(BLACK);
  }
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
      auto card = _buyMenu->GetRootElement<Base::UIStackPanel>()
                    ->GetChild<Base::UIStackPanel>("card-stack")
                    ->GetChild<Base::UIStackPanel>(std::format("card{0}", i));
      auto cost = card->GetChild<Base::UIStackPanel>("price-container")->GetChild<Base::UILabel>("light-cost");
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
    auto card = _buyMenu->GetRootElement<Base::UIStackPanel>()
                  ->GetChild<Base::UIStackPanel>("card-stack")
                  ->GetChild<Base::UIStackPanel>(std::format("card{0}", i));
    if (currentItems[i].cost > player->GetComponent<LightCollectorComponent>()->value)
    {
      alpha = 0.3;
    }
    else
    {
      alpha = 1;
    }
    alphas[i] = alpha;
    GetOwner()->GetTweenManager()->AddTween<float>(                   //
      {card.get(), std::format("alpha-{0}", i)},                      //
      [=](float pos) { card->GetRenderTransform().SetOpacity(pos); }, //
      {
        .startValue = card->GetRenderTransform().GetOpacity(), //
        .endValue = alpha,
        .duration = 0.3,
        .easingType = Base::TweenManager::EasingType::EaseOut,
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
