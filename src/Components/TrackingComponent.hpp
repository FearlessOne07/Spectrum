#pragma once
#include <base/components/Component.hpp>
#include <cstddef>

struct TrackingComponent : public Base::Component
{
  bool isTracking = true;
  size_t targetEntityID = 0.f;
  float trackingDistance = 0.f;
};
