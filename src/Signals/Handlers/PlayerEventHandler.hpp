#pragma once
#include "base/scenes/Scene.hpp"
#include <base/signals/Signal.hpp>
#include <memory>

class PlayerSignalHandler
{
private: // Handlers
  Base::Scene *_scene = nullptr;
  void PlayerEnemyCollisionHandler(const std::shared_ptr<Base::Signal>);
  void PlayerDeathHandler(const std::shared_ptr<Base::Signal>);

public:
  // Function to regsier events
  void Init(Base::Scene *scene);
};
