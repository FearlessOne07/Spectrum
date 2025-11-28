#include "Spawner.hpp"
#include "Components/DamageComponent.hpp"
#include "Components/DiveComponent.hpp"
#include "Components/EnemyComponent.hpp"
#include "Components/HealthComponent.hpp"
#include "Components/KnockBackComponent.hpp"
#include "Components/LightCollectorComponent.hpp"
#include "Components/LightComponent.hpp"
#include "Components/ShootComponent.hpp"
#include "Components/Tags/HealthPacker.hpp"
#include "Components/Tags/PlayerTag.hpp"
#include "Components/TrackingComponent.hpp"
#include "Components/TransformEffects.hpp"
#include "Signals/EntityDiedSignal.hpp"
#include "Signals/PlayerSpawnedSignal.hpp"
#include "base/components/AnimationComponent.hpp"
#include "base/components/AreaComponent.hpp"
#include "base/components/ProximityComponent.hpp"
#include "base/components/SpriteComponent.hpp"
#include "base/components/StateComponent.hpp"
#include "base/components/TimerComponent.hpp"
#include "base/input/MouseButtons.hpp"
#include "base/signals/SignalBus.hpp"
#include "base/sprites/Sprite.hpp"
#include "base/state/TransitionConditionBlock.hpp"
#include "base/util/Ref.hpp"
#include <base/assets/AssetManager.hpp>
#include <base/components/ColliderComponent.hpp>
#include <base/components/ImpulseComponent.hpp>
#include <base/components/InputComponent.hpp>
#include <base/components/MoveComponent.hpp>
#include <base/components/RigidBodyComponent.hpp>
#include <base/components/ShapeComponent.hpp>
#include <base/components/TransformComponent.hpp>
#include <base/entities/Entity.hpp>
#include <base/entities/EntityManager.hpp>
#include <base/renderer/RenderContextSingleton.hpp>
#include <base/scenes/SceneLayer.tpp>
#include <memory>
#include <random>
#include <raylib.h>

void Spawner::SetToSpawn(std::vector<EnemySpec> toSpawn, float difficultyScale)
{
  for (EnemySpec &spec : toSpawn)
  {
    _difficultyScale = difficultyScale;
    _toSpawn.push(spec);
  }
}

void Spawner::Init(const Base::SceneLayer *parentLayer, Base::Ref<Base::EntityManager> entityManager)
{
  _parentLayer = parentLayer;
  _entityManager = entityManager;
  _spawnOffset = float(200 * _parentLayer->GetCameraZoom());

  auto bus = Base::SignalBus::GetInstance();
  bus->SubscribeSignal<EntityDiedSignal>([this](std::shared_ptr<Base::Signal> sig) {
    this->SpawnLight(std::static_pointer_cast<EntityDiedSignal>(sig)); //
  });
}

int Spawner::GetToSpawnCount() const
{
  return _toSpawn.size();
}

Base::EntityID Spawner::SpawnPlayer(Vector2 position, const Ship &ship)
{
  auto e = _entityManager->CreateEntity();
  const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();

  auto transcmp = e->GetComponent<Base::TransformComponent>();
  transcmp->position = position;

  auto transfxcmp = e->AddComponent<TransformEffectsComponent>();
  transfxcmp->targetAngularVelocity = 180;
  transfxcmp->angularAcceleration = 1;
  transfxcmp->bind = true;
  transfxcmp->bindMin = _parentLayer->GetScreenToWorld({0, 0});
  transfxcmp->bindMax = _parentLayer->GetScreenToWorld(_parentLayer->GetSize());
  transfxcmp->rotate = true;

  auto mvcmp = e->AddComponent<Base::MoveComponent>();
  mvcmp->driveForce = 1500;
  mvcmp->brakeForce = 500.f;

  auto rbcmp = e->AddComponent<Base::RigidBodyComponent>();
  rbcmp->isKinematic = false;
  rbcmp->mass = 1;
  rbcmp->drag = 3;

  auto hlthcmp = e->AddComponent<HealthComponent>(20.f);

  auto shtcmp = e->AddComponent<ShootComponent>();
  shtcmp->bulletFireRate = 0.6;
  shtcmp->bulletLifetime = 3;
  shtcmp->bulletFireTimer = 1;
  shtcmp->bulletSpeed = 1500.f;

  auto sprtcmp = e->AddComponent<Base::SpriteComponent>( //
    Base::Sprite{
      _parentLayer->GetAsset<Base::Texture>("ships"),
      Vector2{ship.SpriteSource.x, ship.SpriteSource.y},
      Vector2{ship.SpriteSource.width, ship.SpriteSource.height},
      Vector2{64, 64},
    } //
  );
  shtcmp->bulletSprite = {
    _parentLayer->GetAsset<Base::Texture>("entities"),
    {0, 8},
    Vector2{8, 8},
    Vector2{32, 32},
  };

  auto abbcmp = e->AddComponent<Base::ColliderComponent>();
  abbcmp->radius = sprtcmp->GetTargetSize().x / 2;
  abbcmp->shape = Base::ColliderComponent::Shape::CIRCLE;

  auto inpcmp = e->AddComponent<Base::InputComponent>();
  inpcmp->BindKeyDown(Base::Key::A, [rbcmp]() { rbcmp->direction.x = -1; });
  inpcmp->BindKeyDown(Base::Key::D, [rbcmp]() { rbcmp->direction.x = 1; });
  inpcmp->BindKeyDown(Base::Key::W, [rbcmp]() { rbcmp->direction.y = -1; });
  inpcmp->BindKeyDown(Base::Key::S, [rbcmp]() { rbcmp->direction.y = 1; });

  inpcmp->BindKeyReleased(Base::Key::A, [rbcmp]() { rbcmp->direction.x = 0; });
  inpcmp->BindKeyReleased(Base::Key::D, [rbcmp]() { rbcmp->direction.x = 0; });
  inpcmp->BindKeyReleased(Base::Key::S, [rbcmp]() { rbcmp->direction.y = 0; });
  inpcmp->BindKeyReleased(Base::Key::W, [rbcmp]() { rbcmp->direction.y = 0; });

  inpcmp->BindMouseButtonDown(Base::MouseKey::Left, [this, shtcmp]() {
    const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();
    shtcmp->IsFiring = true;
    shtcmp->targetPosition = _parentLayer->GetLayerCameraMousePosition();
  });
  inpcmp->BindMouseButtonReleased(Base::MouseKey::Left, [shtcmp]() { shtcmp->IsFiring = false; });

  auto dmgcmp = e->AddComponent<DamageComponent>(1);
  e->AddComponent<Base::ImpulseComponent>();
  e->AddComponent<PlayerTag>();

  auto lightColCmp = e->AddComponent<LightCollectorComponent>();
  float baseRadius = abbcmp->radius / _parentLayer->GetCameraZoom();
  lightColCmp->collectionRaius = (baseRadius + (baseRadius * 0.5));
  _entityManager->AddEntity(e);

  auto bus = Base::SignalBus::GetInstance();
  std::shared_ptr<PlayerSpawnedSignal> sig = std::make_shared<PlayerSpawnedSignal>();
  sig->player = e;
  bus->BroadCastSignal(sig);

  _playerID = e->GetID();
  return _playerID;
}

void Spawner::SpawnWave( //
  float dt,
  Base::EntityID playerID //
)
{
  if (_spawnTimer >= _spawnDuration && !_toSpawn.empty())
  {
    std::random_device rd;
    std::mt19937_64 gen(rd());

    _spawnTimer = 0.f;
    const Base::RenderContext *rctx = Base::RenderContextSingleton::GetInstance();

    EnemySpec &spec = _toSpawn.front();
    _toSpawn.pop();

    std::uniform_int_distribution sideDist(1, 4);
    int side = sideDist(gen);

    Vector2 min = {-_spawnOffset, -_spawnOffset};
    Vector2 max = {_parentLayer->GetSize().x + _spawnOffset, _parentLayer->GetSize().y + _spawnOffset};

    Vector2 position = {0, 0};
    float ypos = 0.f;
    float xpos = 0.f;

    switch (side)
    {
    case 1: // Right
      ypos = std::uniform_real_distribution<float>(min.y, max.y)(gen);
      position = _parentLayer->GetScreenToWorld({max.x, ypos});
      break;
    case 2: // Left
      ypos = std::uniform_real_distribution<float>(min.y, max.y)(gen);
      position = _parentLayer->GetScreenToWorld({min.x, ypos});
      break;
    case 3: // Up
      xpos = std::uniform_real_distribution<float>(min.x, max.x)(gen);
      position = _parentLayer->GetScreenToWorld({xpos, min.y});
      break;
    case 4: // Down
      xpos = std::uniform_real_distribution<float>(min.x, max.x)(gen);
      position = _parentLayer->GetScreenToWorld({xpos, max.y});
      break;
    }

    auto e = _entityManager->CreateEntity();

    auto enemcmp = e->AddComponent<EnemyComponent>();
    enemcmp->type = spec.Type;
    enemcmp->value = spec.Value;

    auto transcmp = e->GetComponent<Base::TransformComponent>();
    transcmp->position = position;

    e->AddComponent<Base::ImpulseComponent>();

    auto mvcmp = e->AddComponent<Base::MoveComponent>();

    auto rbcmp = e->AddComponent<Base::RigidBodyComponent>();
    rbcmp->isKinematic = false;
    rbcmp->drag = 3;
    rbcmp->mass = 1;

    auto abbcmp = e->AddComponent<Base::ColliderComponent>();
    abbcmp->shape = Base::ColliderComponent::Shape::CIRCLE;

    auto transfxcmp = e->AddComponent<TransformEffectsComponent>();
    transfxcmp->rotate = true;
    transfxcmp->angularAcceleration = 0.5;
    transfxcmp->targetAngularVelocity = 90;
    transfxcmp->bind = false;
    transfxcmp->lookAt = true;
    transfxcmp->lookAtTarget = _playerID;

    if (spec.IsHealthPacker)
    {
      transfxcmp->lookAt = false;
      e->AddComponent<HealthPacker>();
    }

    e->AddComponent<KnockBackComponent>(1000);
    e->AddComponent<HealthComponent>(spec.BaseHealth * _difficultyScale);
    e->AddComponent<DamageComponent>(spec.BaseDamage * _difficultyScale);

    // Sprite
    std::shared_ptr<Base::SpriteComponent> sprtcmp = nullptr;
    Vector2 worldMin = _parentLayer->GetScreenToWorld({abbcmp->radius, abbcmp->radius});
    Vector2 worldMax = _parentLayer->GetScreenToWorld(_parentLayer->GetSize());

    Rectangle screenWorldArea = {
      worldMin.x,
      worldMin.y,
      worldMax.x - worldMin.x,
      worldMax.y - worldMin.y,
    };

    switch (spec.Type)
    {
    case EnemyType::CHASER: {
      auto trckcmp = e->AddComponent<TrackingComponent>(_playerID);
      sprtcmp = e->AddComponent<Base::SpriteComponent>( //
        Base::Sprite{
          _parentLayer->GetAsset<Base::Texture>("entities"),
          Vector2{0, 0},
          Vector2{8, 8},
          Vector2{64, 64},
        } //
      );
      mvcmp->driveForce = std::uniform_real_distribution<float>(400, 500)(gen);
      break;
    }
    case EnemyType::SHOOTER: {
      sprtcmp = e->AddComponent<Base::SpriteComponent>( //
        Base::Sprite{
          _parentLayer->GetAsset<Base::Texture>("entities"),
          Vector2{8, 0},
          Vector2{8, 8},
          Vector2{64, 64},
        } //
      );
      mvcmp->driveForce = 350;

      auto shtcmp = e->AddComponent<ShootComponent>();
      shtcmp->bulletFireRate = 5.f;
      shtcmp->bulletKnockbackForce = 800;
      shtcmp->bulletSpeed = 1000.f;
      shtcmp->bulletSprite = {
        _parentLayer->GetAsset<Base::Texture>("entities"),
        {8, 8},
        Vector2{8, 8},
        Vector2{32, 32},
      };
      transfxcmp->bindMin = _parentLayer->GetScreenToWorld({0, 0});
      transfxcmp->bindMax = _parentLayer->GetScreenToWorld(_parentLayer->GetSize());

      std::unordered_map<std::string, Base::State> states = //
        {
          {
            "fire",
            Base::State{
              Base::ComponentBundle{
                [this](std::shared_ptr<Base::Entity> e) {
                  auto shtcmp = e->GetComponent<ShootComponent>();
                  auto rbcmp = e->GetComponent<Base::RigidBodyComponent>();
                  shtcmp->IsFiring = true;
                  shtcmp->targetEntity = _playerID;
                  rbcmp->direction = {0, 0};
                },
                [](std::shared_ptr<Base::Entity> e) {
                  auto shtcmp = e->GetComponent<ShootComponent>();
                  shtcmp->IsFiring = false;
                  shtcmp->bulletFireTimer = 0;
                },
              },
              Base::TransitionConditionBlock{
                "chase",
                Base::TransitionEvaluationType::OR,
                std::make_shared<Base::ProximityExit>(_playerID, 250 / _parentLayer->GetCameraZoom()),
                std::make_shared<Base::AreaExit>(screenWorldArea),
              },
            },
          },
          {
            "chase",
            Base::State{
              Base::ComponentBundle{
                [](std::shared_ptr<Base::Entity>) {},
                [](std::shared_ptr<Base::Entity>) {},
                std::make_shared<TrackingComponent>(_playerID),
              },
              Base::TransitionConditionBlock{
                "fire",
                Base::TransitionEvaluationType::AND,
                std::make_shared<Base::ProximityEntry>(_playerID, 500 / _parentLayer->GetCameraZoom()),
                std::make_shared<Base::AreaEntry>(screenWorldArea),
              },
            },
          },
        };

      auto statecmp = e->AddComponent<Base::StateComponent>("chase", states);
      break;
    }
    case EnemyType::KAMIKAZE: {
      mvcmp->driveForce = 500;
      sprtcmp = e->AddComponent<Base::SpriteComponent>( //
        Base::Sprite{
          _parentLayer->GetAsset<Base::Texture>("entities"),
          Vector2{16, 0},
          Vector2{8, 8},
          Vector2{64, 64},
        } //
      );

      std::unordered_map<std::string, Base::State> states = {
        {
          "chase",
          Base::State{
            Base::ComponentBundle{
              [](std::shared_ptr<Base::Entity> e) {},
              [](std::shared_ptr<Base::Entity>) {},
              std::make_shared<TrackingComponent>(_playerID),
            },
            Base::TransitionConditionBlock{
              "scope",
              Base::TransitionEvaluationType::AND,
              std::make_shared<Base::AreaEntry>(screenWorldArea),
            },
          },
        },
        {
          "scope",
          Base::State{
            Base::ComponentBundle{
              [](std::shared_ptr<Base::Entity> e) {
                auto rbcmp = e->GetComponent<Base::RigidBodyComponent>();
                rbcmp->direction = {0, 0};
              },
              [](std::shared_ptr<Base::Entity>) {},
            },
            Base::TransitionConditionBlock{
              "dive",
              Base::TransitionEvaluationType::OR,
              std::make_shared<Base::TimerComponent>(5.f),
            },
          },
        },
        {
          "dive",
          Base::State{
            Base::ComponentBundle{
              [](std::shared_ptr<Base::Entity> e) {
                auto mvcmp = e->GetComponent<Base::MoveComponent>();
                mvcmp->driveForce *= 6;

                auto dvcmp = e->GetComponent<DiveComponent>();
                dvcmp->ResetHasDived();

                auto transfxcmp = e->GetComponent<TransformEffectsComponent>();
                transfxcmp->rotate = false;
              },
              [](std::shared_ptr<Base::Entity> e) {
                auto mvcmp = e->GetComponent<Base::MoveComponent>();
                mvcmp->driveForce /= 6;

                auto transfxcmp = e->GetComponent<TransformEffectsComponent>();
                transfxcmp->rotate = true;
              },
              std::make_shared<DiveComponent>(_playerID),
            },
            Base::TransitionConditionBlock{
              "chase",
              Base::TransitionEvaluationType::OR,
              std::make_shared<Base::AreaExit>(screenWorldArea),
            },
          },
        },
      };
      auto statecmp = e->AddComponent<Base::StateComponent>("chase", states);
      break;
    }
    case EnemyType::NONE:
      break;
    }

    abbcmp->radius = sprtcmp->GetTargetSize().x / 2;

    _entityManager->AddEntity(e);
  }
  else
  {
    _spawnTimer += dt;
  }
}

void Spawner::SpawnLight(std::shared_ptr<EntityDiedSignal> sig)
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
    float targetSize = 16 + (lightValue - 1) * 4;

    auto e = _entityManager->CreateEntity();
    e->GetComponent<Base::TransformComponent>()->position =
      sig->entity->GetComponent<Base::TransformComponent>()->position;

    auto colcmp = e->AddComponent<Base::ColliderComponent>();
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
    _entityManager->AddEntity(e);
  }
}
