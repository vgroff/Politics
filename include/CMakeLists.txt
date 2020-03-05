add_subdirectory(economy)
add_subdirectory(common)
add_subdirectory(electors)


target_sources(politics
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/Engine.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Nation.hpp"
)