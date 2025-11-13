
#include "Shop/Shop.hpp"
#include "base/entities/Entity.hpp"
#include "base/ui/UILayer.hpp"
#include "base/util/Ref.hpp"
#include <base/scenes/SceneLayer.hpp>
#include <memory>

class GameUILayer : public Base::SceneLayer
{
private:
  void InitPauseMenu();
  void Pause();
  void UnPause();
  void InitHud(std::shared_ptr<Base::Entity> player);

  // Shop
  void InitShopMenu(std::shared_ptr<Base::Entity> player);
  void OpenShop();
  void CloseShop();
  std::array<float, 3> UpdateItems();
  bool BuyItem(int index);

  Base::Ref<Base::UILayer> _hud;
  Base::Ref<Base::UILayer> _pauseMenu;
  Base::Ref<Base::UILayer> _buyMenu;

  Shop _shop;

public:
  void Update(float dt) override;
  void Render() override;
  void OnAttach() override;
  void OnDetach() override;
  void OnInputEvent(std::shared_ptr<Base::InputEvent> &event) override;
};
