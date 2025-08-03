#pragma once
#include "base/scenes/SceneLayer.hpp"
#include <base/signals/Signal.hpp>
#include <memory>

class EntitySignalHandler
{
private: // Handlers
  Base::SceneLayer *_parentLayer = nullptr;
  void CollisionHandler(const std::shared_ptr<Base::Signal>);
  void DeathHandler(const std::shared_ptr<Base::Signal>);
  void DamagedHandler(const std::shared_ptr<Base::Signal>);

public:
  // Function to regsier events
  void Init(Base::SceneLayer *scene);
};
