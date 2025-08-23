#pragma once
#include "base/systems/System.hpp"
class DiveSystem : public Base::System
{
public:
  virtual void Update(float dt, Base::EntityManager *entityManager, const Base::Scene *) override;
};
