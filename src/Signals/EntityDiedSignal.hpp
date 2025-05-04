#include "base/entities/Entity.hpp"
#include "base/signals/Signal.hpp"

class EntityDiedSignal : public Base::Signal
{
public:
  std::shared_ptr<Base::Entity> entity = nullptr;
};
