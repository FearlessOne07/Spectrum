#pragma once
#include "base/assets/AssetHandle.hpp"
#include "base/shaders/Shader.hpp"
#include "base/shaders/ShaderEffect.hpp"
#include "raylib.h"

class Vignette : public Base::ShaderEffect
{
  Base::AssetHandle<Base::BaseShader> _vignetteShader;
  Color _vignetteColor = WHITE;

public:
  void Setup(const Base::Scene *layer) override;
  void Apply(RenderTexture2D *input, RenderTexture2D *output, Vector2 resolution) override;

  Vignette(Color vignetteColor);
};
