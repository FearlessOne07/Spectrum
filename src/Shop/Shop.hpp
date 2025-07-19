#pragma once

#include "Modifiers/Modifier.hpp"
#include <array>
#include <cstdint>
#include <memory>
struct ShopItem
{
  uint16_t cost = 0;
  std::shared_ptr<Modifier> modifier = nullptr;
  bool locked = false;
  bool bought = false;
};

class Shop
{
private:
  std::array<ShopItem, 3> _currentItems;

public:
  void RefreshShop();
  const ShopItem &BuyItem(int index);
  const std::array<ShopItem, 3> &GetItems();
};
