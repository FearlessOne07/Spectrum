#include "Vignette.hpp"
#include "base/scenes/Scene.hpp"
#include <algorithm>

Vignette::Vignette(Color vignetteColor, float decayRate, float maxStrength)
  : _vignetteColor(vignetteColor), _decayRate(decayRate), _maxVignetteStrength((std::clamp<float>(maxStrength, 0, 1)))
{
}

void Vignette::Setup(std::weak_ptr<const Base::Scene> scene)
{
  _currentScene = scene;
  _vignetteShader = scene.lock()->GetAsset<Base::BaseShader>("vignette");
}

void Vignette::Apply(RenderTexture2D *input, RenderTexture2D *output, Vector2 resolution)
{
  auto shaderMan = _currentScene.lock()->GetShaderManager();
  BeginTextureMode(*output);
  shaderMan->ActivateShader(_vignetteShader);
  shaderMan->SetUniform(_vignetteShader, "uVignetteColor", _vignetteColor);
  shaderMan->SetUniform(_vignetteShader, "uResolution", resolution);
  shaderMan->SetUniform(_vignetteShader, "uVignetteStrength", _vignetteStrength);
  DrawTexturePro( //
    input->texture, {0, 0, resolution.x, -resolution.y}, {0, 0, resolution.x, resolution.y}, {0, 0}, 0,
    WHITE //
  );
  shaderMan->DeactivateCurrentShader();
  EndTextureMode();
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
