#pragma once
#include "Ship/Ship.hpp"
#include "base/entities/Entity.hpp"
struct SharedGameData
{
  Base::EntityID PlayerId;
  Ship PlayerShip;
};
