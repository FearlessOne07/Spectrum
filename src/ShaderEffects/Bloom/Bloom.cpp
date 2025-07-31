#include "Bloom.hpp"
#include "base/renderer/RenderLayer.hpp"
#include "base/scenes/Scene.hpp"
#include "raylib.h"

void Bloom::SetUpBuffers(Vector2 resolution)
{
  _blurPassX = LoadRenderTexture(resolution.x, resolution.y);
  _blurPassY = LoadRenderTexture(resolution.x, resolution.y);
  _brightPass = LoadRenderTexture(resolution.x, resolution.y);
  _buffersSetUp = true;
}

void Bloom::Setup(const Base::RenderLayer *layer)
{
  _renderLayer = layer;

  _brightShader = layer->GetOwnerScene()->GetAsset<Base::BaseShader>("bright_pass");
  _blurShader = layer->GetOwnerScene()->GetAsset<Base::BaseShader>("blur_pass");
  _combineShader = layer->GetOwnerScene()->GetAsset<Base::BaseShader>("combine_pass");
}

void Bloom::Apply(RenderTexture2D *input, RenderTexture2D *output, Vector2 resolution)
{
  auto shaderMan = _renderLayer->GetOwnerScene()->GetShaderManager();
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
  shaderMan->SetUniform(_blurShader, "resolution", resolution);
  shaderMan->SetUniform(_blurShader, "direction", Vector2{1, 0});
  DrawTexturePro( //
    _brightPass.texture, {0, 0, resolution.x, -resolution.y}, {0, 0, resolution.x, resolution.y}, {0, 0}, 0,
    WHITE //
  );
  shaderMan->DeactivateCurrentShader();
  EndTextureMode();

  BeginTextureMode(_blurPassY);
  shaderMan->ActivateShader(_blurShader);
  shaderMan->SetUniform(_blurShader, "resolution", resolution);
  shaderMan->SetUniform(_blurShader, "direction", Vector2{0, 1});
  DrawTexturePro( //
    _blurPassX.texture, {0, 0, resolution.x, -resolution.y}, {0, 0, resolution.x, resolution.y}, {0, 0}, 0,
    WHITE //
  );
  shaderMan->DeactivateCurrentShader();
  EndTextureMode();

  BeginTextureMode(*output);
  shaderMan->ActivateShader(_combineShader);
  shaderMan->SetUniform(_combineShader, "texture1", _blurPassY.texture);
  shaderMan->SetUniform(_combineShader, "bloomIntensity", _bloomIntensitiy);
  DrawTexturePro( //
    input->texture, {0, 0, resolution.x, -resolution.y}, {0, 0, resolution.x, resolution.y}, {0, 0}, 0,
    WHITE //
  );
  shaderMan->DeactivateCurrentShader();
  EndTextureMode();
}
