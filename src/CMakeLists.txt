set(SOURCES
  "./main.cpp"
)
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories("${PROJECT_NAME}"
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
)
target_link_libraries("${PROJECT_NAME}"
)
