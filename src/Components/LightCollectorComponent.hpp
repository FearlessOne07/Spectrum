#include "base/components/Component.hpp"

struct LightCollectorComponent : public Base::Component
{
  int value = 0;
  float collectionRaius = 0;
};
