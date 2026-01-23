#include "base/scenes/Scene.hpp"
#include <base/signals/Signal.hpp>
#include <base/systems/System.hpp>
#include <memory>

class BulletSystem : public Base::System
{
private:
  void Start() override;

private: // Hanlders
  void EntityCollisionHandler(const std::shared_ptr<Base::Signal> &);

public:
  void Update(                                                                                        //
    float dt, Base::Ref<Base::EntityManager> entityManager, std::shared_ptr<Base::Scene> currentScene //
    ) override;
};
