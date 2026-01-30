#pragma once
#include "base/assets/AssetHandle.hpp"
#include "base/assets/AssetLayout.hpp"
#include "base/assets/AssetManager.hpp"
#include "base/audio/Sound.hpp"
#include "base/util/Ref.hpp"
struct GameSceneAssets
{
  Base::AssetHandle<Base::Texture> PowerUps;
  Base::AssetHandle<Base::Texture> HeartUi;
  Base::AssetHandle<Base::Texture> Entities;
  Base::AssetHandle<Base::Sound> BulletFire;
  Base::AssetHandle<Base::Sound> EnemyDie;
  Base::AssetHandle<Base::Sound> PlayerHit;
  Base::AssetHandle<Base::Shader> Vignette;
};

template <> struct Base::AssetLayout<GameSceneAssets>
{
  static void Load(Ref<AssetManager> assets, GameSceneAssets &out)
  {
    // Texures
    out.PowerUps = assets->LoadTexture("assets/textures/power-ups.png");
    out.HeartUi = assets->LoadTexture("assets/textures/heart-ui.png");
    out.Entities = assets->LoadTexture("assets/textures/entities.png");

    // Sounds
    out.BulletFire = assets->LoadSound("assets/sounds/bullet-fire.wav");
    out.EnemyDie = assets->LoadSound("assets/sounds/enemy-die.wav");
    out.PlayerHit = assets->LoadSound("assets/sounds/player-hit.wav");

    // Shaders
    out.Vignette = assets->LoadShader(Base::ShaderPath{
      "",
      "assets/shaders/vignette/vignette.frag",
      Base::GeometryType::Texture,
    });
  }
};
