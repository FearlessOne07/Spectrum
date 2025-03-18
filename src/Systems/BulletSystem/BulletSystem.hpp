#include "base/System.hpp"

class BulletSystem : public Base::System
{
private:
  void Start() override;
  void Stop() override;

public:
  virtual void Update(float dt, Base::EntityManager *entityManager) override;
};
