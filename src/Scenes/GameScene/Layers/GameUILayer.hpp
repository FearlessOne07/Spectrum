
#include "Shop/Shop.hpp"
#include "base/signals/Signal.hpp"
#include "base/ui/UILayer.hpp"
#include <base/scenes/SceneLayer.hpp>
#include <memory>

class GameUILayer : public Base::SceneLayer
{
private:
  void OnPlayerDamaged(std::shared_ptr<Base::Signal>);
  void InitPauseMenu();
  void InitHud();
  void InitShopMenu();
  void OpenShop();
  void CloseShop();
  bool BuyItem(int index);

  Base::UILayer *_hud;
  Base::UILayer *_pauseMenu;
  Base::UILayer *_buyMenu;

  Shop _shop;

public:
  void Update(float dt) override;
  void Render() override;
  void OnAttach() override;
  void OnDetach() override;
  void OnInputEvent(std::shared_ptr<Base::InputEvent> &event) override;
};
