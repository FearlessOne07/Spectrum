set(SOURCES
  "./main.cpp"
  "./Scenes/GameScene/GameScene.cpp"
  "./Scenes/GameScene/Layers/MainGameLayer.cpp"
  "./Scenes/GameScene/Layers/ParticleLayer.cpp"
  "./Scenes/GameScene/Layers/GameUILayer.cpp"

  "./Scenes/MainMenu/MainMenu.cpp"
  "./Scenes/MainMenu/Layers/MainMenuLayer.cpp"

  "./Scenes/DeathScreen/Layers/MainLayer.cpp"
  "./Scenes/DeathScreen/DeathScreen.cpp"

  "./Systems/BulletSystem/BulletSystem.cpp"
  "./Systems/TrackingSystem/TrackingSystem.cpp"
  "./Systems/HealthSystem/HealthSystem.cpp"
  "./Systems/TransformEffectsSystem/TransformEffectsSystem.cpp"
  "./Systems/PowerUpSystem/PowerUpSystem.cpp"

  "./Signals/Handlers/PlayerEventHandler.cpp"

  "./Spawner/Spawner.cpp"
  "./WaveManager/WaveManager.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories("${PROJECT_NAME}"
  PUBLIC
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/vendor/Base/include"
)

target_link_libraries("${PROJECT_NAME}"
  PRIVATE
    base
)
