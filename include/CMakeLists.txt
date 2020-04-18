add_subdirectory(economy)
add_subdirectory(common)
add_subdirectory(electors)
add_subdirectory(politics)
add_subdirectory(gui)



target_sources(politics
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/App.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Log.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Engine.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Nation.hpp"
)