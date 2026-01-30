#pragma once
#include "base/assets/AssetHandle.hpp"
#include "base/assets/AssetLayout.hpp"
#include "base/assets/AssetManager.hpp"
#include "base/assets/Font.hpp"
#include "base/audio/AudioStream.hpp"
#include "base/shaders/Shader.hpp"
#include "base/util/Ref.hpp"
struct GlobalAssets
{
  Base::AssetHandle<Base::Font> MainFont;

  Base::AssetHandle<Base::AudioStream> GameTrack;
  Base::AssetHandle<Base::AudioStream> MainMenuTrack;

  Base::AssetHandle<Base::Texture> Button;
  Base::AssetHandle<Base::Texture> Ships;

  Base::AssetHandle<Base::Shader> BlurPass;
  Base::AssetHandle<Base::Shader> CombinePass;
  Base::AssetHandle<Base::Shader> BrightPass;
};

template <> struct Base::AssetLayout<GlobalAssets>
{
  static void Load(Base::Ref<AssetManager> assets, GlobalAssets &out)
  {
    out.MainFont = assets->LoadFont("assets/fonts/main-font.ttf", true);

    out.GameTrack = assets->LoadAudioStream("assets/music/game-track.mp3", true);
    out.MainMenuTrack = assets->LoadAudioStream("assets/music/main-menu-track.wav", true);

    out.Button = assets->LoadTexture("assets/textures/button.png", true);
    out.Ships = assets->LoadTexture("assets/textures/ships.png", true);

    out.BlurPass = assets->LoadShader(
      Base::ShaderPath{
        "",
        "assets/shaders/bloom/blur_pass.frag",
        Base::GeometryType::Texture,
      },
      true //
    );
    out.CombinePass = assets->LoadShader(
      Base::ShaderPath{
        "",
        "assets/shaders/bloom/combine_pass.frag",
        Base::GeometryType::Texture,
      },
      true //
    );
    out.BrightPass = assets->LoadShader(
      Base::ShaderPath{
        "",
        "assets/shaders/bloom/bright_pass.frag",
        Base::GeometryType::Texture,
      },
      true //
    );
  }
};
