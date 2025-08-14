#include "WaveManager.hpp"
#include "Components/EnemyComponent.hpp"
#include "Components/LightComponent.hpp"
#include "Components/TransformEffects.hpp"
#include "Spawner/Spawner.hpp"
#include "base/components/ColliderComponent.hpp"
#include "base/components/ImpulseComponent.hpp"
#include "base/components/MoveComponent.hpp"
#include "base/components/RigidBodyComponent.hpp"
#include "base/components/SpriteComponent.hpp"
#include "base/components/TransformComponent.hpp"
#include "base/entities/Entity.hpp"
#include "base/scenes/SceneLayer.hpp"
#include "base/signals/Signal.hpp"
#include "base/signals/SignalBus.hpp"
#include <base/assets/AssetManager.hpp>
#include <base/entities/EntityManager.hpp>
#include <base/scenes/SceneLayer.inl>
#include <memory>
#include <random>

void WaveManager::Init(const Base::SceneLayer *parentLayer, Base::EntityManager *entityMan)
{
  _entityMan = entityMan;
  _parentLayer = parentLayer;
  _spawner = Spawner(_parentLayer);
  auto bus = Base::SignalBus::GetInstance();
  bus->SubscribeSignal<EntityDiedSignal>([this](std::shared_ptr<Base::Signal> sig) {
    this->SpawnLight(std::static_pointer_cast<EntityDiedSignal>(sig)); //
  });
}

void WaveManager::GenerateWave()
{
  std::random_device _rd;
  std::mt19937_64 _gen = std::mt19937_64(_rd());

  // Init Variables
  int aliveCount = _entityMan->Query<EnemyComponent>().size();

  // Setup
  _enemiesToSpawn.clear();
  _wavePoints = (_baseWavePoints * _currentWave) - (3 * aliveCount);

  // Create pool of available enemies for this wave
  std::unordered_map<EnemyType, EnemySpec> pool = {};
  for (auto &[type, spec] : _enemySpawnInfo)
  {
    if (_currentWave >= spec.unlockWave && spec.cost <= _wavePoints)
    {
      pool[type] = spec;
    }
  }

  // Safety check - if no enemies are available, exit
  if (pool.empty())
  {
    _currentWave++;
    return; // Avoid infinite loop if no enemies can be spawned
  }

  // Calculate total chance for weighted selection
  float totalChance = 0;
  for (auto &[type, spec] : pool)
  {
    totalChance += spec.spawnChance;
  }

  while (_wavePoints > 0)
  {
    bool canAffordAny = false;
    for (auto &[type, spec] : pool)
    {
      if (spec.cost <= _wavePoints)
      {
        canAffordAny = true;
        break;
      }
    }

    if (!canAffordAny || pool.empty())
    {
      break;
    }

    float selectedChance = std::uniform_real_distribution<float>(0, totalChance)(_gen);
    float runningSum = 0;
    bool enemySelected = false;

    for (auto &[type, spec] : pool)
    {
      if (spec.cost <= _wavePoints)
      {
        runningSum += spec.spawnChance;
        if (selectedChance <= runningSum)
        {
          _wavePoints -= spec.cost;
          _enemiesToSpawn.push_back(type); // Fixed: removed asterisks
          enemySelected = true;
          break;
        }
      }
    }

    if (!enemySelected)
    {
      EnemyType cheapestType;
      int lowestCost = INT_MAX;

      for (auto &[type, spec] : pool)
      {
        if (spec.cost <= _wavePoints && spec.cost < lowestCost)
        {
          lowestCost = spec.cost;
          cheapestType = type;
        }
      }

      if (lowestCost != INT_MAX)
      {
        _wavePoints -= pool[cheapestType].cost;
        _enemiesToSpawn.push_back(cheapestType);
      }
    }
  }

  _spawner.SetToSpawn(_enemiesToSpawn);
  _currentWave++;
}

void WaveManager::SpawnWaves(float dt)
{
  int count = _entityMan->Query<EnemyComponent>().size();
  if (_waveTimer >= _waveTime || (count == 0 && _spawner.GetToSpawnCount() == 0))
  {
    _waveTimer = 0;
    GenerateWave();
  }
  else
  {
    _waveTimer += dt;
  }
  _spawner.SpawnWave(dt, _entityMan, _playerID);
}

Base::EntityID WaveManager::SpawnPlayer()
{
  _playerID = _spawner.SpawnPlayer(_entityMan, {0, 0});
  return _playerID;
}

void WaveManager::SpawnLight(std::shared_ptr<EntityDiedSignal> sig)
{
  std::random_device _rd;
  std::mt19937_64 _gen = std::mt19937_64(_rd());

  if (sig->entity->HasComponent<EnemyComponent>())
  {
    int value = sig->entity->GetComponent<EnemyComponent>()->value;

    for (int i = 0; i < value; i++)
    {
      auto e = _entityMan->CreateEntity();
      e->GetComponent<Base::TransformComponent>()->position =
        sig->entity->GetComponent<Base::TransformComponent>()->position;

      auto colcmp = e->AddComponent<Base::ColliderComponent>();
      colcmp->SetTypeFlag(Base::ColliderComponent::Type::HITBOX);
      colcmp->shape = Base::ColliderComponent::Shape::CIRCLE;
      colcmp->radius = 8;

      auto sprtcmp = e->AddComponent<Base::SpriteComponent>(                                               //
        _parentLayer->GetAsset<Base::Texture>("power-ups"), Vector2{16, 8}, Vector2{8, 8}, Vector2{16, 16} //
      );

      auto rbcmp = e->AddComponent<Base::RigidBodyComponent>();
      rbcmp->isKinematic = false;
      rbcmp->drag = 4;
      rbcmp->mass = 1;

      auto mvcmp = e->AddComponent<Base::MoveComponent>();
      mvcmp->driveForce = 0;

      float angle = std::uniform_real_distribution<float>(0, 2 * PI)(_gen);
      rbcmp->direction = {sin(angle), cos(angle)};

      auto impcmp = e->AddComponent<Base::ImpulseComponent>();
      impcmp->force = std::uniform_int_distribution<int>(200, 400)(_gen);
      impcmp->direction = {sin(angle), cos(angle)};

      auto transfx = e->AddComponent<TransformEffectsComponent>();
      transfx->bind = true;
      transfx->bindMin = _parentLayer->GetScreenToWorld({0, 0});
      transfx->bindMax = _parentLayer->GetScreenToWorld(_parentLayer->GetSize());

      e->AddComponent<LightComponent>()->value = 1;
      _entityMan->AddEntity(e);
    }
  }
}

WaveManager::~WaveManager()
{
}
