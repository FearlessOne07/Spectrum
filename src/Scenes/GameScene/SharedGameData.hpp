#pragma once
#include "Ship/ShipDataBase.hpp"
#include "base/entities/Entity.hpp"
struct SharedGameData
{
  Base::EntityID PlayerId;
  ShipName PlayerShip;
};
