#pragma once

#include "Modifiers/Modifier.hpp"
#include "base/scenes/SceneLayer.hpp"
#include "base/sprites/NinePatchSprite.hpp"
#include <array>
#include <cstdint>
#include <memory>
#include <random>
#include <vector>
struct ShopItem
{
  std::shared_ptr<Modifier> modifier = nullptr;
  Base::NinePatchSprite textureIcon;
  uint16_t cost = 0;
  bool locked = false;
  bool bought = true;
  std::string name = "";
};

struct StockItem
{
  std::shared_ptr<Modifier> modifier = nullptr;
  Base::NinePatchSprite itemSprite;
  int modifierLevel = 1;
  std::string name = "";
};

class Shop
{
private:
  std::array<ShopItem, 3> _currentItems = {};
  std::vector<StockItem> _stock;
  Base::SceneLayer *_ownerLayer = nullptr;
  bool _newItems = false;

  std::random_device _rd;
  std::mt19937_64 _gen;

public:
  void Init(Base::SceneLayer *ownerLayer);
  void RefreshShop();
  void BuyItem(int index);
  const std::array<ShopItem, 3> &GetItems();
  const ShopItem &GetItem(int index);
  bool HasNewItems() const;
  void ResetNewItems();
};
