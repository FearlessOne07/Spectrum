#pragma once
#include <base/systems/System.hpp>

class HealthSystem : public Base::System
{
public:
  void Update(                                                                                        //
    float dt, Base::Ref<Base::EntityManager> entityManager, std::shared_ptr<Base::Scene> currentScene //
    ) override;
};
