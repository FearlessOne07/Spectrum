#pragma once

#include "Ship/Ship.hpp"
#include <unordered_map>
enum class ShipName
{
  Yellow,
  White
};

class ShipDataBase
{
  static inline std::unordered_map<ShipName, Ship> _ships = {
    {
      ShipName::White,
      Ship{
        .SpriteSource = {{0, 0}, {8, 8}},
        .BulletSpriteSource = {{0, 8}, {8, 8}},
        .Damage = 1,
        .Health = 20.f,
        .FireRate = 0.6,
        .BulletSpeed = 1500,
        .DriveForce = 1500,
      },
    },
    {
      ShipName::Yellow,
      Ship{
        .SpriteSource = {{8, 0}, {8, 8}},
        .BulletSpriteSource = {{8, 8}, {8, 8}},
        .Damage = 2,
        .Health = 15,
        .FireRate = 0.9,
        .BulletSpeed = 1000,
        .DriveForce = 1000,
      },
    },
  };

public:
  static const Ship &GetShip(const ShipName &name)
  {
    return _ships.at(name);
  }
};
