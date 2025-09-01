#include "base/systems/System.hpp"

class DiveSystem : public Base::System
{
public:
  void Update(float dt, Base::EntityManager *entityManager, const Base::Scene *) override;
};
