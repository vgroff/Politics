add_subdirectory(economy)
add_subdirectory(common)
add_subdirectory(electors)
add_subdirectory(politics)



target_sources(politics
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/App.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Engine.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Nation.hpp"
)