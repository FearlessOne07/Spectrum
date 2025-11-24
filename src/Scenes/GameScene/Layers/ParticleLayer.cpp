#include "ParticleLayer.hpp"
#include "Components/EnemyComponent.hpp"
#include "Scenes/GameScene/Signals/GamePause.hpp"
#include "Scenes/GameScene/Signals/GameResume.hpp"
#include "base/components/SpriteComponent.hpp"
#include "base/components/TransformComponent.hpp"
#include "base/input/Events/MouseButtonEvent.hpp"
#include "base/input/InputEvent.hpp"
#include "base/particles/ParticleEmitter.hpp"
#include "base/renderer/RenderContextSingleton.hpp"
#include "base/scenes/Scene.hpp"
#include "base/signals/SignalBus.hpp"
#include "base/sprites/Sprite.hpp"
#include "raylib.h"
#include <base/audio/signals/PlaySoundSignal.hpp>
#include <base/particles/ParticleManager.hpp>
#include <memory>
#include <random>

void ParticleLayer::OnAttach()
{
  _gen = std::mt19937_64(_rd());
  _emitter = GetOwner()->GetParticleManager()->AddEmitter();
  _emitter->burst = true;
  _emitter->emissionType = Base::ParticleEmitter::EmissionType::Point;
  _emitter->particleShape = Base::ParticleEmitter::ParticleShape::Texture;
  _emitter->burstEmissionCount = 60;
  _emitter->particleRotationSpeed = 180;
  _emitter->isEmitting = false;
  _emitter->SetPauseMask(GetPauseMask());
  _emitter->particleEndColor = {255, 255, 255, 255};

  _testEmitter = GetOwner()->GetParticleManager()->AddEmitter();
  _testEmitter->burst = true;
  _testEmitter->emissionType = Base::ParticleEmitter::EmissionType::Point;
  _testEmitter->particleShape = Base::ParticleEmitter::ParticleShape::Circle;
  _testEmitter->particleStartRadius = 20;
  _testEmitter->particleEndRadius = 0;
  _testEmitter->burstEmissionCount = 60;
  _testEmitter->particleRotationSpeed = 180;
  _testEmitter->isEmitting = false;
  _testEmitter->particleEndColor = {0, 0, 0, 0};
  _testEmitter->SetPauseMask(GetPauseMask());

  auto bus = Base::SignalBus::GetInstance();
  bus->SubscribeSignal<EntityDiedSignal>([this](std::shared_ptr<Base::Signal> sig) {
    this->OnEntityDiedSignal(std::static_pointer_cast<EntityDiedSignal>(sig));
  });
  bus->SubscribeSignal<GamePausedSignal>([this](std::shared_ptr<Base::Signal>) { Pause(); });
  bus->SubscribeSignal<GameResumedSignal>([this](std::shared_ptr<Base::Signal>) { UnPause(); });
}

void ParticleLayer::OnDetach()
{
}

void ParticleLayer::Update(float dt)
{
}

void ParticleLayer::Render()
{
  const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();
  BeginCamera();
  GetOwner()->GetParticleManager()->Render();
  EndCamera();
}

void ParticleLayer::OnInputEvent(std::shared_ptr<Base::InputEvent> &event)
{
  if (auto mouseEvent = std::dynamic_pointer_cast<Base::MouseButtonEvent>(event))
  {
    if (mouseEvent->Button == Base::MouseKey::Left && mouseEvent->action == Base::MouseButtonEvent::Action::Pressed)
    {
      std::uniform_real_distribution<float> lifeRange(0.5, 1);
      std::uniform_real_distribution<float> angleDist(0, 360);
      std::uniform_real_distribution<float> speedDist(0, 700);

      _testEmitter->isEmitting = true;
      _testEmitter->initialisationFunction = [=, this](Base::ParticleEmitter &emitter) mutable {
        emitter.particleLifeTime = lifeRange(_gen);
        emitter.particleStartSpeed = speedDist(_gen);
        float angle = angleDist(_gen);
        emitter.particleDirection = {
          static_cast<float>(sin(angle * DEG2RAD)),
          static_cast<float>(cos(angle * DEG2RAD)),
        };
        emitter.emissionPoint = GetLayerCameraMousePosition();
      };
    }
  }
}

void ParticleLayer::OnEntityDiedSignal(std::shared_ptr<EntityDiedSignal> signal)
{
  if (signal->entity->HasComponent<EnemyComponent>())
  {
    auto e = signal->entity;
    auto sprtcmp = e->GetComponent<Base::SpriteComponent>();
    auto transcmp = e->GetComponent<Base::TransformComponent>();
    std::uniform_real_distribution<float> lifeRange(0.5, 1);
    std::uniform_real_distribution<float> radiusRange(                   //
      0.2 * sprtcmp->GetTargetSize().x, 0.3 * sprtcmp->GetTargetSize().x //
    );
    std::uniform_real_distribution<float> angleDist(0, 360);
    std::uniform_real_distribution<float> speedDist(0, 700);

    _emitter->isEmitting = true;
    _emitter->initialisationFunction = [=, this](Base::ParticleEmitter &emitter) mutable {
      emitter.particleSprite = Base::Sprite(sprtcmp->GetSprite());
      emitter.particleLifeTime = lifeRange(_gen);
      float size = radiusRange(_gen) * 2;
      emitter.particleStartSize = {size, size};
      emitter.particleStartSpeed = speedDist(_gen);

      float angle = angleDist(_gen);
      emitter.particleDirection = {static_cast<float>(sin(angle * DEG2RAD)), static_cast<float>(cos(angle * DEG2RAD))};
      emitter.emissionPoint = transcmp->position;
    };

    Base::CameraShakeConfig config;
    config.trauma = 0.5;
    config.frequency = 50.0f;
    config.shakeMagnitude = GetSize().x * (0.05);
    config.duration = 1;
    config.traumaMultiplyer = 2;
    config.rotationMagnitude = 0.5;
    ShakeCamera(config);

    auto bus = Base::SignalBus::GetInstance();
    std::shared_ptr<Base::PlaySoundSignal> sig = std::make_shared<Base::PlaySoundSignal>();
    sig->soundHandle = GetOwner()->GetAsset<Base::Sound>("enemy-die");
    bus->BroadCastSignal(sig);
  }
}
