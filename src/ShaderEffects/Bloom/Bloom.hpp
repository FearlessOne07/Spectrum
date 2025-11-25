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
  float _luminanceThreshHold = 0;
  float _bloomIntensitiy = 0;
  float _blurResolutionScale = 0;
  Vector2 _blurResolution = {0, 0};

private:
  void SetUpBuffers(Vector2 resolution);

public:
  void Setup(std::weak_ptr<const Base::Scene> shaderManager) override;
  void Apply(RenderTexture2D *input, RenderTexture2D *output, Vector2 resolution) override;

  Bloom(float bloomIntensity, float luminanceThresh, float blurResolutionScale);

  // Uniforms
  void SetBlurResoltuionScale(float res);
  void SetLuminanceThresh(float thresh);
  void SetBloomIntensity(float thresh);
};
