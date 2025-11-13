#include "Signals/EntityDiedSignal.hpp"
#include <base/particles/ParticleEmitter.hpp>
#include <base/scenes/SceneLayer.hpp>
#include <random>

class ParticleLayer : public Base::SceneLayer
{
  std::random_device _rd;
  std::mt19937_64 _gen;
  Base::ParticleEmitter *_emitter = nullptr;

  Base::ParticleEmitter *_testEmitter = nullptr;

  void OnEntityDiedSignal(std::shared_ptr<EntityDiedSignal> signal);

public:
  void Update(float dt) override;
  void Render() override;
  void OnAttach() override;
  void OnDetach() override;
  void OnInputEvent(std::shared_ptr<Base::InputEvent> &event) override;
};
