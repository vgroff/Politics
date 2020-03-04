add_subdirectory(Economy)

target_sources(politics
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/Engine.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Nation.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Pop.cpp"
)