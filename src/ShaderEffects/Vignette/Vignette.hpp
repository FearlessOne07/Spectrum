#pragma once
#include "base/rendering/FrameBuffer.hpp"
#include "base/rendering/Material.hpp"
#include "base/shaders/ShaderEffect.hpp"
#include "base/util/Colors.hpp"
#include <memory>

class Vignette : public Base::ShaderEffect
{
  Base::Material _vignetteMaterial;
  Base::Color _vignetteColor = Base::White;
  float _vignetteStrength = 0;
  float _decayRate = 0;
  float _minVignetteStrenght = 0;
  float _maxVignetteStrength = 1;
  float _maintain = false;

public:
  Vignette(Base::Color vignetteColor, float decayRate, float maxStrenth);
  void Setup(std::weak_ptr<Base::Scene> layer) override;
  void Apply( //
    Base::Ptr<Base::FrameBuffer> input, Base::Ptr<Base::FrameBuffer> output,
    Base::Vector2 resolution //
    ) override;
  void Update(float dt) override;
  void Flash();
  void SetMinStrength(float min);
  void SetMaintain(bool maintain);
  void SetVignetteColor(Base::Color color);
};
