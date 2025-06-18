#include "base/scenes/Scene.hpp"
#include <base/signals/Signal.hpp>
#include <base/systems/System.hpp>

class PowerUpSystem : public Base::System
{
public:
  void Update(float dt, Base::EntityManager *entityManager, const Base::Scene *) override;
};
