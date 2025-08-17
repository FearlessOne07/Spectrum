#pragma once
#include "base/assets/AssetHandle.hpp"
#include "base/entities/Entity.hpp"
#include "base/assets/AssetHandle.hpp"
#include "base/textures/Texture.hpp"
#include <base/components/Component.hpp>
#include <raylib.h>

struct ShootComponent : public Base::Component
{
  Vector2 targetPosition = {0, 0};
  Base::EntityID targetEntity;
  float bulletFireRate = 0.f;
  float bulletFireTimer = 0.f;
  bool IsFiring = false;

  // Bullet spec
  float bulletKnockbackForce = 0.f;
  float bulletLifetime = 5;
  float bulletSpeed = 0.f;
  Base::AssetHandle<Base::Texture> bulletTexture;
};
