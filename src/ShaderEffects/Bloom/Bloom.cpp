#include "Bloom.hpp"
#include "base/rendering/FrameBuffer.hpp"
#include "base/rendering/FramebufferAttachmentIndex.hpp"
#include "base/scenes/Scene.hpp"
#include "base/util/Type.hpp"
#include <algorithm>

Bloom::Bloom(float bloomIntensity, float luminanceThresh, float blurResolution)
{
  SetBlurResoltuionScale(blurResolution);
  SetBloomIntensity(bloomIntensity);
  SetLuminanceThresh(luminanceThresh);
}

void Bloom::SetUpBuffers(Base::IVector2 resolution)
{
  _blurResolution = Base::Vector2{resolution} * _blurResolutionScale;
  _blurPassX = Base::FrameBuffer::Create({.Width = _blurResolution.x, .Height = _blurResolution.y});
  _blurPassY = Base::FrameBuffer::Create({.Width = _blurResolution.x, .Height = _blurResolution.y});
  _brightPass = Base::FrameBuffer::Create({.Width = resolution.x, .Height = resolution.y});
  _buffersSetUp = true;
}

void Bloom::Setup(std::weak_ptr<Base::Scene> scene)
{
  _currentScene = scene;
  _brightMaterial = {scene.lock()->Engine().Assets->GetAsset<Base::Shader>("bright_pass", true).Get()};
  _blurMaterial = {scene.lock()->Engine().Assets->GetAsset<Base::Shader>("blur_pass", true).Get()};
  _combineMaterial = {scene.lock()->Engine().Assets->GetAsset<Base::Shader>("combine_pass", true).Get()};
}

void Bloom::Apply(Base::Ptr<Base::FrameBuffer> input, Base::Ptr<Base::FrameBuffer> output, Base::Vector2 resolution)
{
  if (!_buffersSetUp)
  {
    SetUpBuffers(resolution);
  }

  _brightMaterial.SetUniform("threshold", _luminanceThreshHold);
  BeginFrameBuffer(_brightPass);
  DrawFrameBuffer(input, {resolution.x, resolution.y}, _brightMaterial, Base::FramebufferAttachmentIndex::Color0);
  EndFrameBuffer();

  _blurMaterial.SetUniform("resolution", _blurResolution);
  _blurMaterial.SetUniform("direction", Base::Vector2{1, 0});
  BeginFrameBuffer(_blurPassX);
  DrawFrameBuffer( //
    _brightPass, {_blurResolution.x, _blurResolution.y}, _blurMaterial,
    Base::FramebufferAttachmentIndex::Color0 //
  );
  EndFrameBuffer();

  _blurMaterial.SetUniform("resolution", _blurResolution);
  _blurMaterial.SetUniform("direction", Base::Vector2{0, 1});
  BeginFrameBuffer(_blurPassY);
  DrawFrameBuffer( //
    _blurPassX, {_blurResolution.x, _blurResolution.y}, _blurMaterial,
    Base::FramebufferAttachmentIndex::Color0 //
  );
  EndFrameBuffer();

  _combineMaterial.SetTexture(1, _blurPassY->GetColorAttachment(Base::FramebufferAttachmentIndex::Color0));
  _combineMaterial.SetUniform("bloomIntensity", _bloomIntensitiy);

  BeginFrameBuffer(output);
  DrawFrameBuffer(input, {resolution.x, resolution.y}, _combineMaterial, Base::FramebufferAttachmentIndex::Color0);
  EndFrameBuffer();
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
