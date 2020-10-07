add_subdirectory(economy)
add_subdirectory(common)
add_subdirectory(electors)
add_subdirectory(politics)
add_subdirectory(gui)


target_sources(politics
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/App.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Clock.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Engine.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Nation.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Log.cpp"
)