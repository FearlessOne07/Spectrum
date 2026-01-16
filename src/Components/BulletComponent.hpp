#include <base/components/Component.hpp>
#include <base/util/Type.hpp>

struct BulletComponent : public Base::Component
{
  float lifeTimer = 0.f;
  float lifeTime = 0.f;
  Base::Vector2 target = {0, 0};
  std::shared_ptr<Base::Entity> sender = nullptr;
};
