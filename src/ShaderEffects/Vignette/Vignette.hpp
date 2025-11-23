#pragma once
#include "base/assets/AssetHandle.hpp"
#include "base/shaders/Shader.hpp"
#include "base/shaders/ShaderEffect.hpp"
#include "raylib.h"
#include <memory>

class Vignette : public Base::ShaderEffect
{
  Base::AssetHandle<Base::BaseShader> _vignetteShader;
  Color _vignetteColor = WHITE;
  float _vignetteStrength = 0;
  float _decayRate = 0;
  float _minVignetteStrenght = 0;
  float _maxVignetteStrength = 1;
  float _maintain = false;

public:
  Vignette(Color vignetteColor, float decayRate, float maxStrenth);
  void Setup(std::weak_ptr<const Base::Scene> layer) override;
  void Apply(RenderTexture2D *input, RenderTexture2D *output, Vector2 resolution) override;
  void Update(float dt) override;
  void Flash();
  void SetMinStrength(float min);
  void SetMaintain(bool maintain);
};
