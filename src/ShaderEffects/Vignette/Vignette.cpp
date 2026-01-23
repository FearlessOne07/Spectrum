#include "Vignette.hpp"
#include "base/scenes/Scene.hpp"
#include "base/util/Colors.hpp"
#include <algorithm>

Vignette::Vignette(Base::Color vignetteColor, float decayRate, float maxStrength)
  : _vignetteColor(vignetteColor), _decayRate(decayRate), _maxVignetteStrength((std::clamp<float>(maxStrength, 0, 1)))
{
}

void Vignette::Setup(std::weak_ptr<Base::Scene> scene)
{
  _currentScene = scene;
  _vignetteMaterial = scene.lock()->Engine().Assets->GetAsset<Base::Shader>("vignette").Get();
}

void Vignette::Apply(Base::Ptr<Base::FrameBuffer> input, Base::Ptr<Base::FrameBuffer> output, Base::Vector2 resolution)
{
  BeginFrameBuffer(output);
  ClearFrameBuffer(Base::Blank);
  _vignetteMaterial.SetUniform("uVignetteColor", _vignetteColor);
  _vignetteMaterial.SetUniform("uResolution", resolution);
  _vignetteMaterial.SetUniform("uVignetteStrength", _vignetteStrength);
  DrawFrameBuffer(input, {resolution.x, resolution.y}, _vignetteMaterial, Base::FramebufferAttachmentIndex::Color0);
  EndFrameBuffer();
}

void Vignette::Update(float dt)
{
  _vignetteStrength -= _decayRate * dt;
  if (_vignetteStrength <= _minVignetteStrenght)
  {
    _vignetteStrength = _minVignetteStrenght;

    if (!_maintain)
    {
      _active = false;
    }
  }
}

void Vignette::Flash()
{
  _vignetteStrength = _maxVignetteStrength;
  _active = true;
}

void Vignette::SetMinStrength(float min)
{
  _minVignetteStrenght = std::min(min, 1.f);
}

void Vignette::SetMaintain(bool maintain)
{
  _maintain = maintain;
}

void Vignette::SetVignetteColor(Base::Color color)
{
  _vignetteColor = color;
}
