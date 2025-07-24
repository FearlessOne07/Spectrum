#pragma once
#include <algorithm>
#include <base/components/Component.hpp>
#include <cmath>

struct HealthComponent : public Base::Component
{
private:
  float _health = 0;
  float _maxHealth = 0;
  float _sickness = 0;
  bool _healthChanged = false;
  float _lastDamageTaken = 0;

public:
  HealthComponent(float maxHealth) : _maxHealth(maxHealth), _health(maxHealth)
  {
  }

  float GetHealth() const
  {
    return _health;
  }

  float GetMaxHealth() const
  {
    return _maxHealth;
  }

  void SetHealth(float health)
  {
    _health = health;
    _health = std::clamp<float>(_health, 0, _maxHealth);
    _health = std::round(_health);
    _healthChanged = true;
  }

  void SetMaxHealth(float maxHealth)
  {
    _maxHealth = maxHealth;
    _maxHealth = std::max<float>(_maxHealth, 1);
    _maxHealth = std::round(_maxHealth);
    _healthChanged = true;
  }

  void TakeDamage(float damage)
  {
    _health -= damage;
    _health = std::max(_health, 0.f);
    _lastDamageTaken = damage;
    _healthChanged = true;
  }

  bool HealthChanged() const
  {
    return _healthChanged;
  };

  void Reset()
  {
    _healthChanged = false;
    _lastDamageTaken = 0;
  }

  float GetLastTakenDamage()
  {
    return _lastDamageTaken;
  }
};
