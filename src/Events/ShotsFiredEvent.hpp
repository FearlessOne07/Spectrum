#include "base/Entity.hpp"
#include "base/Event.hpp"

struct ShotsFiredEvent : public Base::Event
{
public:
  Base::Entity *shooter;
};
