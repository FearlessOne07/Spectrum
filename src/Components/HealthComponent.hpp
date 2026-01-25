#pragma once
#include <algorithm>
#include <base/components/Component.hpp>
#include <cmath>

struct HealthComponent : public Base::Component
{
private:
  float _health = 0;
  float _maxHealth = 0;
  float _baseHealth = 0;
  float _sickness = 0;
  bool _healthChanged = false;
  bool _tookDamage = false;
  float _lastDamageTaken = 0;

public:
  HealthComponent(float baseHealth) : _baseHealth(baseHealth), _maxHealth(baseHealth), _health(baseHealth)
  {
  }

  float GetHealth() const
  {
    return _health;
  }

  float GetBaseHealth() const
  {
    return _baseHealth;
  }

  void Kill()
  {
    TakeDamage(_health);
  }

  float GetMaxHealth() const
  {
    return _maxHealth;
  }

  void SetHealth(float health)
  {
    _health = health;
    _health = std::clamp<float>(_health, 0, _maxHealth);
    _healthChanged = true;
  }

  void SetMaxHealth(float maxHealth)
  {
    _maxHealth = maxHealth;
    _maxHealth = std::max<float>(_maxHealth, 1);
    _healthChanged = true;
  }

  void TakeDamage(float damage)
  {
    _health -= damage;
    _health = std::max(_health, 0.f);
    _lastDamageTaken = damage;
    _tookDamage = true;
  }

  bool HealthChanged() const
  {
    return _healthChanged;
  };

  bool TookDamage() const
  {
    return _tookDamage;
  }

  void Reset()
  {
    _healthChanged = false;
    _tookDamage = false;
    _lastDamageTaken = 0;
  }

  float GetLastTakenDamage()
  {
    return std::round(_lastDamageTaken * 100.f) / 100.f;
  }
};
