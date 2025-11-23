#pragma once
#include <base/systems/System.hpp>

class TrackingSystem : public Base::System
{
public:
  void Update(                                                                                              //
    float dt, Base::Ref<Base::EntityManager> entityManager, std::shared_ptr<const Base::Scene> currentScene //
    ) override;
};
