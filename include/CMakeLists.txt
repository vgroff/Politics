add_subdirectory(Economy)

target_sources(politics
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/Engine.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Nation.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Pop.hpp"
)