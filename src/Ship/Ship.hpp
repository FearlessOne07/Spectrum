#include "base/assets/AssetHandle.hpp"
#include "base/textures/Texture.hpp"
#include "raylib.h"

struct Ship
{
  Base::AssetHandle<Base::Texture> texture;
  Vector2 sourcePos = {0, 0};
};
