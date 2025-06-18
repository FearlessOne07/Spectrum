#pragma once
#include "Modifiers/Modifier.hpp"
#include "base/entities/Entity.hpp"
#include <base/components/Component.hpp>
#include <memory>
#include <vector>
struct PowerUp
{
private:
  std::shared_ptr<Modifier> _modifier;
  float _duration = 0;
  bool _isApplied = false;

public:
  PowerUp(float duration, std::shared_ptr<Modifier> modifer) : _modifier(modifer), _duration(duration)
  {
  }

  bool IsApplied() const
  {
    return _isApplied;
  }

  void Apply(std::shared_ptr<Base::Entity> entity)
  {
    _isApplied = true;
    _modifier->Apply(entity);
  }

  void Remove(std::shared_ptr<Base::Entity> e)
  {
    _modifier->Remove(e);
  }

  float GetTimeLeft() const
  {
    return _duration;
  }

  void DecrmentDuration(float dt)
  {
    _duration -= dt;
  }
};

struct PowerUpComponent : public Base::Component
{
private:
  std::vector<PowerUp> _powerUps;

public:
  template <typename T> void AddPowerUp(float duration)
  {
    if (std::is_base_of_v<Modifier, T>)
    {
      PowerUp powerUp(duration, std::make_shared<T>());
      _powerUps.push_back(powerUp);
    }
  }

  std::vector<PowerUp> &GetPowerUps()
  {
    return _powerUps;
  }

  std::vector<PowerUp>::iterator RemovePowerUp(std::vector<PowerUp>::iterator it)
  {
    return _powerUps.erase(it);
  }
};
