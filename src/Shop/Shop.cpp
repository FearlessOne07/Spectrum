#include "Shop.hpp"
#include "Modifiers/HealthBoost/HealthBoostModifier.hpp"
#include "Modifiers/MaxHealthModifier/MaxHealthModifier.hpp"
#include "Modifiers/SpeedModifier/SpeedModifier.hpp"
#include "base/scenes/SceneLayer.hpp"
#include <algorithm>
#include <cmath>
#include <memory>
#include <random>

void Shop::Init(Base::SceneLayer *ownerLayer)
{
  _gen = std::mt19937_64(_rd());
  _ownerLayer = ownerLayer;

  // Init Stock
  std::shared_ptr<SpeedModifier> speed = std::make_shared<SpeedModifier>();
  std::uniform_int_distribution basePriceDist(3, 7);
  speed->SetSpeedBoost(0.02);
  _stock.emplace_back( //
    speed,
    Base::NinePatchSprite{
      _ownerLayer->GetAsset<Base::Texture>("power-ups"),
      {},
      {5, 1},
      {8, 8},
    },
    1, "Speed", basePriceDist(_gen) //
  );

  std::shared_ptr<MaxHealthModifier> maxHealth = std::make_shared<MaxHealthModifier>();
  maxHealth->SetMaxHealthBoost(0.2);
  _stock.emplace_back( //
    maxHealth,
    Base::NinePatchSprite{
      _ownerLayer->GetAsset<Base::Texture>("heart-ui"),
      {},
      {2, 0},
      {8, 8},
    },
    1, "Max Health", basePriceDist(_gen) //
  );

  std::shared_ptr<HealthBoostModifier> health = std::make_shared<HealthBoostModifier>();
  health->SetHealthBoost(5);
  _stock.emplace_back( //
    health,
    Base::NinePatchSprite{
      _ownerLayer->GetAsset<Base::Texture>("heart-ui"),
      {},
      {4, 0},
      {8, 8},
    },
    1, "Heal", basePriceDist(_gen) //
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

      if (item.stockItem)
      {
        item.stockItem->modifierLevel++;
      }

      StockItem &sItem = _stock[stockDist(_gen)];

      // while (true)
      // {
      //   if ( //
      //     std::ranges::find_if(_currentItems, [sItem](ShopItem &shop) { return shop.name == sItem.name; }) ==
      //     _currentItems.end())
      //   {
      //     break;
      //   }
      //   sItem = _stock[stockDist(_gen)];
      // }

      item.bought = false;
      item.cost = std::round(sItem.basePrice * std::pow(1.8, sItem.modifierLevel - 1));
      item.modifier = sItem.modifier;
      item.textureIcon = sItem.itemSprite;
      item.name = sItem.name;
      item.stockItem = &sItem;
    }
  }
}

void Shop::BuyItem(int index)
{
  _currentItems[index].bought = true;
  ShopItem item = _currentItems[index];
  RefreshShop();
}

const std::array<ShopItem, 3> &Shop::GetItems()
{
  return _currentItems;
}

const ShopItem &Shop::GetItem(int index)
{
  return _currentItems[index];
}

bool Shop::HasNewItems() const
{
  return _newItems;
}

void Shop::ResetNewItems()
{
  _newItems = false;
}
