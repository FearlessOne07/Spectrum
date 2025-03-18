set(SOURCES
  "./main.cpp"
  "./Scenes/GameScene/GameScene.cpp"
  "./SpawnManager/SpawnManager.cpp"
  "./Systems/BulletSystem/BulletSystem.cpp"
  "./Systems/TrackingSystem/TrackingSystem.cpp"
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
