#pragma once
#include "base/Event.hpp"

class TimerTickEvent : public Base::Event
{
public:
  float currentTime = 0;
};
