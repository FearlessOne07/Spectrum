#include "Vignette.hpp"
#include "base/scenes/Scene.hpp"

Vignette::Vignette(Color vignetteColor) : _vignetteColor(vignetteColor)
{
}

void Vignette::Setup(const Base::Scene *scene)
{
  _currentScene = scene;
  _vignetteShader = scene->GetAsset<Base::BaseShader>("vignette");
}

void Vignette::Apply(RenderTexture2D *input, RenderTexture2D *output, Vector2 resolution)
{
  auto shaderMan = _currentScene->GetShaderManager();
  BeginTextureMode(*output);
  shaderMan->ActivateShader(_vignetteShader);
  shaderMan->SetUniform(_vignetteShader, "uVignetteColor", _vignetteColor);
  shaderMan->SetUniform(_vignetteShader, "uResolution", resolution);
  DrawTexturePro( //
    input->texture, {0, 0, resolution.x, -resolution.y}, {0, 0, resolution.x, resolution.y}, {0, 0}, 0,
    WHITE //
  );
  shaderMan->DeactivateCurrentShader();
  EndTextureMode();
}
