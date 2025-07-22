#include "Shop.hpp"
#include "Modifiers/MaxHealthModifier/MaxHealthModifier.hpp"
#include "Modifiers/SpeedModifier/SpeedModifier.hpp"
#include "base/scenes/SceneLayer.hpp"
#include <memory>
#include <random>

void Shop::Init(Base::SceneLayer *ownerLayer)
{
  _gen = std::mt19937_64(_rd());
  _ownerLayer = ownerLayer;

  // Init Stock
  std::shared_ptr<SpeedModifier> speed = std::make_shared<SpeedModifier>();
  speed->SetSpeedBoost(0.2);
  _stock.emplace_back( //
    speed,
    Base::NinePatchSprite{
      _ownerLayer->GetAsset<Base::Texture>("power-ups"),
      {},
      {5, 1},
      {8, 8},
    },
    1, "Speed" //
  );

  std::shared_ptr<MaxHealthModifier> health = std::make_shared<MaxHealthModifier>();
  health->SetMaxHealthBoost(0.2);
  _stock.emplace_back( //
    health,
    Base::NinePatchSprite{
      _ownerLayer->GetAsset<Base::Texture>("heart-ui"),
      {},
      {2, 0},
      {8, 8},
    },
    1, "Max Health" //
  );

  RefreshShop();
}

void Shop::RefreshShop()
{
  std::uniform_int_distribution<> stockDist(0, _stock.size() - 1);
  for (auto &item : _currentItems)
  {
    if (item.bought)
    {
      _newItems = true;
      StockItem sItem = _stock[stockDist(_gen)];
      item.bought = false;
      item.cost = std::uniform_int_distribution(5, 20)(_gen);
      item.modifier = sItem.modifier;
      item.textureIcon = sItem.itemSprite;
      item.name = sItem.name;
    }
  }
}

ShopItem Shop::BuyItem(int index)
{
  _currentItems[index].bought = true;
  ShopItem item = _currentItems[index];
  RefreshShop();
  return item;
}

const std::array<ShopItem, 3> &Shop::GetItems()
{
  return _currentItems;
}

bool Shop::HasNewItems() const
{
  return _newItems;
}

void Shop::ResetNewItems()
{
  _newItems = false;
}
