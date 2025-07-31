#include "base/assets/AssetHandle.hpp"
#include "base/shaders/Shader.hpp"
#include "base/shaders/ShaderEffect.hpp"
#include "raylib.h"

class Bloom : public Base::ShaderEffect
{
private:
  RenderTexture2D _blurPassX;
  RenderTexture2D _blurPassY;
  RenderTexture2D _brightPass;
  bool _buffersSetUp = false;

  // Shaders
  Base::AssetHandle<Base::BaseShader> _blurShader;
  Base::AssetHandle<Base::BaseShader> _brightShader;
  Base::AssetHandle<Base::BaseShader> _combineShader;

  // uniforms
  float _luminanceThreshHold = 0.25;
  float _bloomIntensitiy = 0.7f;

private:
  void SetUpBuffers(Vector2 resolution);

public:
  void Setup(const Base::RenderLayer *layer) override;
  void Apply(RenderTexture2D *input, RenderTexture2D *output, Vector2 resolution) override;
};
