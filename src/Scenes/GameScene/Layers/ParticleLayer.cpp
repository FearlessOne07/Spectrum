#include "ParticleLayer.hpp"
#include "base/game/RenderContextSingleton.hpp"
#include "base/input/Events/MouseButtonEvent.hpp"
#include "base/input/InputEvent.hpp"
#include "base/particles/ParticleEmitter.hpp"
#include "base/scenes/Scene.hpp"
#include "raylib.h"
#include <base/game/RenderContext.hpp>
#include <memory>
#include <random>

void ParticleLayer::OnAttach()
{
  auto rctx = Base::RenderContextSingleton::GetInstance();
  _gen = std::mt19937_64(_rd());
  emitter = _owner->GetParticleManager()->AddEmitter();
  emitter->emissionType = Base::ParticleEmitter::EmissionType::POINT;
  emitter->particleShape = Base::ParticleEmitter::ParticleShape::RECT;
  emitter->particleStartSize = {50, 50};
  emitter->particleSideNumber = 4;
  emitter->particleLifeTime = 1.f;
  emitter->emissionRate = 60;
  emitter->isEmitting = false;
  emitter->emissionPoint = GetScreenToWorld2D(rctx->mousePosition, rctx->camera);
}

void ParticleLayer::OnDetach()
{
}

void ParticleLayer::Update(float dt)
{
  emitter->emissionRate += dt * 10;
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
  if (auto mouseEvent = std::dynamic_pointer_cast<Base::MouseButtonEvent>(event))
  {
    auto rctx = Base::RenderContextSingleton::GetInstance();
    if (mouseEvent->action == Base::InputEvent::Action::HELD && mouseEvent->button == MOUSE_BUTTON_RIGHT)
    {
      std::uniform_real_distribution<float> angleDist(0, 360);
      float angle = angleDist(_gen);
      emitter->emissionPoint = GetScreenToWorld2D(rctx->mousePosition, rctx->camera);
      emitter->particleDirection = {static_cast<float>(sin(angle * DEG2RAD)), static_cast<float>(cos(angle * DEG2RAD))};
      emitter->isEmitting = true;
      emitter->particleSpeed = std::uniform_real_distribution<float>(0, 400)(_gen);
      event->isHandled = true;
    }
    else if (mouseEvent->action == Base::InputEvent::Action::RELEASED && mouseEvent->button == MOUSE_BUTTON_RIGHT)
    {
      emitter->isEmitting = false;
      event->isHandled = true;
    }
  }
}
