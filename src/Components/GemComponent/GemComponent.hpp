#pragma once
#include "Gems/Gem.hpp"
#include "base/Component.hpp"
#include <memory>

struct GemComponent : public Base::Component
{
  std::unique_ptr<Gem> currentGem = nullptr;
  std::type_index eqiupRequest = typeid(nullptr);
};
