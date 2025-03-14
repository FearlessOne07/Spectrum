#pragma once
#include "base/System.hpp"

class GemSystem : public Base::System
{
public:
  void Update(float dt, Base::EntityManager *entityManager) override;
};
