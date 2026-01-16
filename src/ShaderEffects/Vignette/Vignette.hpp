#pragma once
#include "base/assets/AssetHandle.hpp"
#include "base/rendering/FrameBuffer.hpp"
#include "base/shaders/Shader.hpp"
#include "base/shaders/ShaderEffect.hpp"
#include "base/util/Colors.hpp"

class Vignette : public Base::ShaderEffect
{
  Base::AssetHandle<Base::Shader> _vignetteShader;
  Base::Color _vignetteColor = Base::White;
  float _vignetteStrength = 0;
  float _decayRate = 0;
  float _minVignetteStrenght = 0;
  float _maxVignetteStrength = 1;
  float _maintain = false;

public:
  Vignette(Base::Color vignetteColor, float decayRate, float maxStrenth);
  void Setup() override;
  void Apply(Base::Ptr<Base::FrameBuffer> input, Base::Ptr<Base::FrameBuffer> output,
             Base::Vector2 resolution) override;
  void Update(float dt) override;
  void Flash();
  void SetMinStrength(float min);
  void SetMaintain(bool maintain);
  void SetVignetteColor(Base::Color color);
};
