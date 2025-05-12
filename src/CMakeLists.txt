set(SOURCES
  "./main.cpp"
  "./Scenes/GameScene/GameScene.cpp"
  "./Scenes/GameScene/Layers/MainGameLayer.cpp"
  "./Scenes/GameScene/Layers/ParticleLayer.cpp"
  "./Scenes/PauseMenu/PauseMenu.cpp"

  "./Scenes/PauseMenu/Layers/MainPauseLayer.cpp"
  "./Scenes/GameScene/Layers/GameUILayer.cpp"

  "./Scenes/DeathScreen/Layers/MainLayer.cpp"
  "./Scenes/DeathScreen/DeathScreen.cpp"

  "./Systems/BulletSystem/BulletSystem.cpp"
  "./Systems/TrackingSystem/TrackingSystem.cpp"
  "./Systems/HealthSystem/HealthSystem.cpp"
  "./Systems/TransformEffectsSystem/TransformEffectsSystem.cpp"


  "./Signals/Handlers/PlayerEventHandler.cpp"

  "./Spawner/Spawner.cpp"
  "./WaveManager/WaveManager.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories("${PROJECT_NAME}"
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/vendor/Base/include"
)

target_link_libraries("${PROJECT_NAME}"
  PRIVATE
    base
)
