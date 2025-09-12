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
#include "base/sprites/Sprite.hpp"
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
  for (auto &spec : _enemySpawnInfo)
  {
    if (_currentWave >= spec.UnlockWave && spec.Cost <= _wavePoints)
    {
      pool[spec.Type] = spec;
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
    totalChance += spec.SpawnChance;
  }

  while (_wavePoints > 0)
  {
    bool canAffordAny = false;
    for (auto &[type, spec] : pool)
    {
      if (spec.Cost <= _wavePoints)
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
      if (spec.Cost <= _wavePoints)
      {
        runningSum += spec.SpawnChance;
        if (selectedChance <= runningSum)
        {
          _wavePoints -= spec.Cost;
          _enemiesToSpawn.push_back(spec); // Fixed: removed asterisks
          enemySelected = true;
          break;
        }
      }
    }

    if (!enemySelected)
    {
      EnemySpec cheapest;
      int lowestCost = INT_MAX;

      for (auto &[type, spec] : pool)
      {
        if (spec.Cost <= _wavePoints && spec.Cost < lowestCost)
        {
          lowestCost = spec.Cost;
          cheapest = spec;
        }
      }

      if (lowestCost != INT_MAX)
      {
        _wavePoints -= cheapest.Cost;
        _enemiesToSpawn.push_back(cheapest);
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
  if (!sig->entity->HasComponent<EnemyComponent>())
  {
    return;
  }

  int totalValue = sig->entity->GetComponent<EnemyComponent>()->value;
  std::random_device _rd;
  std::mt19937_64 _gen(_rd());

  // First, split the value into chunks of at most 3
  std::vector<int> chunks;
  while (totalValue > 0)
  {
    int maxThis = std::min(3, totalValue);

    // Randomly pick between 1 and maxThis
    int v = std::uniform_int_distribution<int>(1, maxThis)(_gen);
    chunks.push_back(v);
    totalValue -= v;
  }

  // Shuffle so it's not always descending
  std::shuffle(chunks.begin(), chunks.end(), _gen);

  // Spawn each chunk as a light entity
  for (int lightValue : chunks)
  {
    float targetSize = 16 + (lightValue - 1) * 4; // 1=16px, 2=20px, 3=24px

    auto e = _entityMan->CreateEntity();
    e->GetComponent<Base::TransformComponent>()->position =
      sig->entity->GetComponent<Base::TransformComponent>()->position;

    auto colcmp = e->AddComponent<Base::ColliderComponent>();
    colcmp->SetTypeFlag(Base::ColliderComponent::Type::HITBOX);
    colcmp->shape = Base::ColliderComponent::Shape::CIRCLE;
    colcmp->radius = targetSize / 2;

    e->AddComponent<Base::SpriteComponent>( //
      Base::Sprite{
        _parentLayer->GetAsset<Base::Texture>("power-ups"),
        Vector2{16, 8},
        Vector2{8, 8},
        Vector2{targetSize, targetSize},
      } //
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

    e->AddComponent<LightComponent>()->value = lightValue;
    _entityMan->AddEntity(e);
  }
}

WaveManager::~WaveManager()
{
}
