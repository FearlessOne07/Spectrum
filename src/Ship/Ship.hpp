#pragma once

#include "base/rendering/Quad.hpp"

enum class DamageType
{
  Melee,
  Ranged
};

struct Ship
{
  // Sprites
  Base::Rectangle SpriteSource;
  Base::Rectangle BulletSpriteSource;

  // Damage
  // DamageType DamageMode = DamageType::Melee;
  float Damage = 0;
  float Health = 0;

  float FireRate = 0;
  float BulletSpeed = 0;

  // Speed
  float DriveForce = 1500;

  // TODO: Specturm Order
  // SpectrumIndex SpectrumOrder = SpectrumIndex::Red;
};
