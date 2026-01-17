#include "Shop.hpp"
#include "Modifiers/DamageBoostModifier/DamageModifier.hpp"
#include "Modifiers/HealthBoost/HealthBoostModifier.hpp"
#include "Modifiers/MaxHealthModifier/MaxHealthModifier.hpp"
#include "Modifiers/SpeedModifier/SpeedModifier.hpp"
#include "base/scenes/Scene.hpp"
#include "base/scenes/SceneLayer.hpp"
#include <algorithm>
#include <cmath>
#include <memory>
#include <random>

void Shop::Init(Base::SceneLayer *ownerLayer)
{
  _gen = std::mt19937(_rd());
  _ownerLayer = ownerLayer;

  // Init Stock
  std::shared_ptr<SpeedModifier> speed = std::make_shared<SpeedModifier>();
  std::uniform_int_distribution basePriceDist(6, 12);
  speed->SetSpeedBoost(0.02);
  _stock.emplace_back( //
    speed,
    Base::NinePatchSprite{
      _ownerLayer->GetOwner()->Engine().Assets->GetAsset<Base::Texture>("power-ups"),
      {},
      {40, 8},
      {8, 8},
    },
    1, L"Speed", basePriceDist(_gen) //
  );

  std::shared_ptr<MaxHealthModifier> maxHealth = std::make_shared<MaxHealthModifier>();
  maxHealth->SetMaxHealthBoost(0.2);
  _stock.emplace_back( //
    maxHealth,
    Base::NinePatchSprite{
      _ownerLayer->GetOwner()->Engine().Assets->GetAsset<Base::Texture>("heart-ui"),
      {},
      {16, 0},
      {8, 8},
    },
    1, L"Max Health", basePriceDist(_gen) //
  );

  std::shared_ptr<HealthBoostModifier> health = std::make_shared<HealthBoostModifier>();
  health->SetHealthBoost(5);
  _stock.emplace_back( //
    health,
    Base::NinePatchSprite{
      _ownerLayer->GetOwner()->Engine().Assets->GetAsset<Base::Texture>("heart-ui"),
      {},
      {32, 0},
      {8, 8},
    },
    1, L"Heal", basePriceDist(_gen) //
  );

  std::shared_ptr<DamageModifier> damageBoost = std::make_shared<DamageModifier>();
  damageBoost->SetDamageBoost(0.15);
  _stock.emplace_back( //
    damageBoost,
    Base::NinePatchSprite{
      _ownerLayer->GetOwner()->Engine().Assets->GetAsset<Base::Texture>("power-ups"),
      {},
      {32, 24},
      {8, 8},
    },
    1, L"Damage Boost", basePriceDist(_gen) //
  );

  RefreshShop();
}

void Shop::RefreshShop()
{
  std::uniform_int_distribution<> stockDist(0, _stock.size() - 1);
  std::uniform_int_distribution<> priceOffset(-3, 3);

  for (auto &item : _currentItems)
  {
    if (item.bought)
    {
      _newItems = true;

      if (item.stockItem)
      {
        item.stockItem->modifierLevel++;
      }

      int newStockIndex = stockDist(_gen);
      // StockItem sItem = _stock[newStockIndex];

      while (true)
      {
        if (                                                                                                         //
          std::ranges::count_if(                                                                                     //
            _currentItems, [this, newStockIndex](ShopItem &shop) { return shop.name == _stock[newStockIndex].name; } //
            ) == 0                                                                                                   //
        )
        {
          break;
        }
        newStockIndex = stockDist(_gen);
      }

      StockItem &sItem = _stock[newStockIndex];

      item.bought = false;

      float basePrice = (sItem.basePrice * std::pow(1.8, sItem.modifierLevel - 1));
      float offset = priceOffset(_gen);
      item.cost = std::round(basePrice + offset);
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
