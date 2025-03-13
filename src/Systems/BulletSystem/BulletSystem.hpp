#include "base/System.hpp"

class BulletSystem : public Base::System
{
public:
  virtual void Update(float dt, Base::EntityManager *entityManager) override;
};
