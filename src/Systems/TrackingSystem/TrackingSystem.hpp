#pragma once
#include "base/System.hpp"

class TrackingSystem : public Base::System
{
private:
  void Start() override;
  void Stop() override;

public:
  void Update(float dt, Base::EntityManager *entityManager) override;
};
