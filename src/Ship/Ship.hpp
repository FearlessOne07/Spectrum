#pragma once
#include "base/assets/AssetHandle.hpp"
#include "base/textures/Texture.hpp"
#include "raylib.h"

struct Ship
{
  Base::AssetHandle<Base::Texture> Texture;
  Vector2 SourcePos = {0, 0};
};
