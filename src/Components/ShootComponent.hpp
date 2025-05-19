#pragma once
#include <base/components/Component.hpp>
#include <raylib.h>

struct ShootComponent : public Base::Component
{
  Vector2 target = {0, 0};
  float bulletFireRate = 0.f;
  float bulletFireTimer = 0.f;
  bool IsFiring = false;

  // Bullet spec
  float bulletKnockbackForce = 0.f;
  float bulletLifetime = 5;
  float bulletSpeed = 0.f;
  std::shared_ptr<Texture> bulletTexture = nullptr;
};
