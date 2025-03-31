#pragma once
#include "base/System.hpp"

class TrackingSystem : public Base::System
{
public:
  void Update(float dt, Base::EntityManager *entityManager) override;
};
