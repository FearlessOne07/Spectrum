#pragma once
#include "base/entities/Entity.hpp"
#include "base/rendering/Sprite.hpp"
#include "base/util/Type.hpp"
#include <base/components/Component.hpp>

struct ShootComponent : public Base::Component
{
  Base::Vector2 targetPosition = {0, 0};
  Base::EntityID targetEntity;
  float bulletFireRate = 0.f;
  float bulletFireTimer = 0.f;
  bool IsFiring = false;

  // Bullet spec
  float bulletKnockbackForce = 0.f;
  float bulletLifetime = 5;
  float bulletSpeed = 0.f;
  Base::Sprite bulletSprite;
};
