#pragma once
#include "base/scenes/SceneLayer.hpp"
#include <base/signals/Signal.hpp>
#include <memory>

class PlayerSignalHandler
{
private: // Handlers
  Base::SceneLayer *_parentLayer = nullptr;
  void PlayerEnemyCollisionHandler(const std::shared_ptr<Base::Signal>);
  void PlayerDeathHandler(const std::shared_ptr<Base::Signal>);

public:
  // Function to regsier events
  void Init(Base::SceneLayer *scene);
};
