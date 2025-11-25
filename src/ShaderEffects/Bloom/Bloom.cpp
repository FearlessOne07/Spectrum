#include "Bloom.hpp"
#include "base/renderer/RenderLayer.hpp"
#include "base/scenes/Scene.hpp"
#include "raylib.h"
#include "raymath.h"
#include <algorithm>

Bloom::Bloom(float bloomIntensity, float luminanceThresh, float blurResolution)
{
  SetBlurResoltuionScale(blurResolution);
  SetBloomIntensity(bloomIntensity);
  SetLuminanceThresh(luminanceThresh);
}

void Bloom::SetUpBuffers(Vector2 resolution)
{
  _blurResolution = resolution * _blurResolutionScale;
  _blurPassX = LoadRenderTexture(_blurResolution.x, _blurResolution.y);
  _blurPassY = LoadRenderTexture(_blurResolution.x, _blurResolution.y);
  _brightPass = LoadRenderTexture(resolution.x, resolution.y);
  _buffersSetUp = true;
}

void Bloom::Setup(std::weak_ptr<const Base::Scene> scene)
{
  _currentScene = scene;
  _brightShader = scene.lock()->GetAsset<Base::BaseShader>("bright_pass");
  _blurShader = scene.lock()->GetAsset<Base::BaseShader>("blur_pass");
  _combineShader = scene.lock()->GetAsset<Base::BaseShader>("combine_pass");
}

void Bloom::Apply(RenderTexture2D *input, RenderTexture2D *output, Vector2 resolution)
{
  auto shaderMan = _currentScene.lock()->GetShaderManager();
  if (!_buffersSetUp)
  {
    SetUpBuffers(resolution);
  }

  BeginTextureMode(_brightPass);
  shaderMan->ActivateShader(_brightShader);
  shaderMan->SetUniform(_brightShader, "threshold", _luminanceThreshHold);
  DrawTexturePro( //
    input->texture, {0, 0, resolution.x, -resolution.y}, {0, 0, resolution.x, resolution.y}, {0, 0}, 0,
    WHITE //
  );
  shaderMan->DeactivateCurrentShader();
  EndTextureMode();

  BeginTextureMode(_blurPassX);
  shaderMan->ActivateShader(_blurShader);
  shaderMan->SetUniform(_blurShader, "resolution", _blurResolution);
  shaderMan->SetUniform(_blurShader, "direction", Vector2{1, 0});
  DrawTexturePro( //
    _brightPass.texture,
    {
      0,
      0,
      resolution.x,
      -resolution.y,
    },
    {0, 0, _blurResolution.x, _blurResolution.y}, {0, 0}, 0,
    WHITE //
  );

  shaderMan->DeactivateCurrentShader();
  EndTextureMode();

  BeginTextureMode(_blurPassY);
  shaderMan->ActivateShader(_blurShader);
  shaderMan->SetUniform(_blurShader, "resolution", _blurResolution);
  shaderMan->SetUniform(_blurShader, "direction", Vector2{0, 1});
  DrawTexturePro( //
    _blurPassX.texture, {0, 0, _blurResolution.x, -_blurResolution.y}, {0, 0, _blurResolution.x, _blurResolution.y},
    {0, 0}, 0,
    WHITE //
  );
  shaderMan->DeactivateCurrentShader();
  EndTextureMode();

  BeginTextureMode(*output);
  shaderMan->ActivateShader(_combineShader);
  shaderMan->SetUniform(_combineShader, "blurredTexture", _blurPassY.texture);
  shaderMan->SetUniform(_combineShader, "bloomIntensity", _bloomIntensitiy);
  DrawTexturePro( //
    input->texture, {0, 0, resolution.x, -resolution.y}, {0, 0, resolution.x, resolution.y}, {0, 0}, 0,
    WHITE //
  );
  shaderMan->DeactivateCurrentShader();
  EndTextureMode();
}

void Bloom::SetLuminanceThresh(float thresh)
{
  _luminanceThreshHold = std::clamp<float>(thresh, 0, 1);
}

void Bloom::SetBloomIntensity(float thresh)
{
  _bloomIntensitiy = std::max<float>(0, thresh);
}

void Bloom::SetBlurResoltuionScale(float res)
{
  _blurResolutionScale = std::clamp<float>(res, 0.25, 1);
}
