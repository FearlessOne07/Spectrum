#pragma once
#include <base/signals/Signal.hpp>
#include <memory>

class PlayerEventHandler
{
private: // Handlers
  void PlayerEnemyCollisionHandler(const std::shared_ptr<Base::Signal>);

public:
  // Function to regsier events
  void Init();
};
