#pragma once
#include <base/Component.hpp>
#include <cstddef>

struct TrackingComponent : public Base::Component
{
  size_t targetEntityID = 0.f;
};
