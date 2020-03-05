add_subdirectory(economy)
add_subdirectory(common)
add_subdirectory(electors)


target_sources(politics
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/Engine.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Nation.cpp"
)