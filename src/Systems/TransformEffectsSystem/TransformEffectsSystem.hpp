#pragma once
#include <base/systems/System.hpp>

class TransformEffectsSystem : public Base::System
{
public:
  void Update(float dt, Base::EntityManager *entityManager) override;
};
