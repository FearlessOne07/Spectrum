#include <base/components/Component.hpp>
#include <raylib.h>

struct BulletComponent : public Base::Component
{
  float lifeTimer = 0.f;
  float lifeTime = 0.f;
  Vector2 target = {.x = 0, .y = 0};
  const Base::Entity *sender = nullptr;
};
