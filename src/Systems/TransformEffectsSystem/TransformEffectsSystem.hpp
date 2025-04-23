#pragma once
#include <base/System.hpp>

class TransformEffectsSystem : public Base::System
{
public:
  void Update(float dt, Base::EntityManager *entityManager) override;
};
