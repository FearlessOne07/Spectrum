#include "base/shaders/ShaderEffect.hpp"

class Bloom : public Base::ShaderEffect
{
private:
  Base::Ptr<Base::FrameBuffer> _blurPassX;
  Base::Ptr<Base::FrameBuffer> _blurPassY;
  Base::Ptr<Base::FrameBuffer> _brightPass;
  bool _buffersSetUp = false;

  // Shaders
  Base::Material _blurMaterial;
  Base::Material _brightMaterial;
  Base::Material _combineMaterial;

  // uniforms
  float _luminanceThreshHold = 0;
  float _bloomIntensitiy = 0;
  float _blurResolutionScale = 0;
  Base::IVector2 _blurResolution = {0, 0};

private:
  void SetUpBuffers(Base::IVector2 resolution);

public:
  void Setup(std::weak_ptr<Base::Scene> scene) override;
  void Apply( //
    Base::Ptr<Base::FrameBuffer> input, Base::Ptr<Base::FrameBuffer> output,
    Base::Vector2 resolution //
    ) override;

  Bloom(float bloomIntensity, float luminanceThresh, float blurResolutionScale);

  // Uniforms
  void SetBlurResoltuionScale(float res);
  void SetLuminanceThresh(float thresh);
  void SetBloomIntensity(float thresh);
};
