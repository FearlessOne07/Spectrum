#pragma once
#include <base/systems/System.hpp>

class TrackingSystem : public Base::System
{
public:
  void Update(float dt, Base::EntityManager *entityManager, const Base::Scene *currentScene) override;
};
