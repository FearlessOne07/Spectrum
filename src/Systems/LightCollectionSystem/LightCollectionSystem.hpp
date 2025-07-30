#include "base/systems/System.hpp"

class LightCollectionSystem : public Base::System
{
public:
  void Update(float dt, Base::EntityManager *entityManager, const Base::Scene *currentScene) override;
};
