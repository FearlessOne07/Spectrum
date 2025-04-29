#pragma once
#include <base/signals/Event.hpp>
#include <memory>

class PlayerEventHandler
{
private: // Handlers
  void PlayerEnemyCollisionHandler(const std::shared_ptr<Base::Event>);

public:
  // Function to regsier events
  void Init();
};
