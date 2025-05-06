#include "ParticleLayer.hpp"
#include "Components/Tags/EnemyTag.hpp"
#include "base/components/ShapeComponent.hpp"
#include "base/components/TransformComponent.hpp"
#include "base/game/RenderContextSingleton.hpp"
#include "base/input/InputEvent.hpp"
#include "base/particles/ParticleEmitter.hpp"
#include "base/scenes/Scene.hpp"
#include "base/signals/SignalManager.hpp"
#include "raylib.h"
#include <base/game/RenderContext.hpp>
#include <memory>
#include <random>

void ParticleLayer::OnAttach()
{
  auto rctx = Base::RenderContextSingleton::GetInstance();
  _gen = std::mt19937_64(_rd());
  _emitter = _owner->GetParticleManager()->AddEmitter();
  _emitter->burst = true;
  _emitter->emissionType = Base::ParticleEmitter::EmissionType::POINT;
  _emitter->particleShape = Base::ParticleEmitter::ParticleShape::POLYGON;
  _emitter->burstEmissionCount = 60;
  _emitter->particleRotationSpeed = 180;
  _emitter->isEmitting = false;

  auto *bus = Base::SignalManager::GetInstance();
  bus->SubscribeSignal<EntityDiedSignal>([this](std::shared_ptr<Base::Signal> sig) {
    this->OnEntityDiedSignal(std::static_pointer_cast<EntityDiedSignal>(sig));
  });
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
  BeginMode2D(rd->camera);
  _owner->GetParticleManager()->Render();
  EndMode2D();
}

void ParticleLayer::OnInputEvent(std::shared_ptr<Base::InputEvent> &event)
{
}

void ParticleLayer::OnEntityDiedSignal(std::shared_ptr<EntityDiedSignal> signal)
{
  if (signal->entity->HasComponent<EnemyTag>())
  {
    auto e = signal->entity;
    auto shpcmp = e->GetComponent<Base::ShapeComponent>();
    auto transcmp = e->GetComponent<Base::TransformComponent>();
    std::uniform_real_distribution<float> lifeRange(0.5, 1);
    std::uniform_real_distribution<float> radiusRange(0.3 * shpcmp->radius, 0.6 * shpcmp->radius);
    std::uniform_real_distribution<float> angleDist(0, 360);
    std::uniform_real_distribution<float> speedDist(0, 700);

    _emitter->isEmitting = true;
    _emitter->particleStartColor = shpcmp->color;
    _emitter->particleSideNumber = shpcmp->points;
    _emitter->initialisationFunction = [=, this](Base::ParticleEmitter &emitter) mutable {
      emitter.particleLifeTime = lifeRange(_gen);
      emitter.particleStartRadius = radiusRange(_gen);
      emitter.particleEndRadius = 0;
      emitter.particleStartSpeed = speedDist(_gen);

      float angle = angleDist(_gen);
      emitter.particleDirection = {static_cast<float>(sin(angle * DEG2RAD)), static_cast<float>(cos(angle * DEG2RAD))};
      emitter.emissionPoint = transcmp->position;
    };
  }
}
