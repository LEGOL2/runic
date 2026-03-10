add_library(runic)
set(SRC
    engine.cppm
    glfw_context.cppm
    program.cppm
    runic.cppm
    utils.cppm
    window.cppm
)
target_sources(runic PUBLIC FILE_SET CXX_MODULES FILES ${SRC})

# glad
add_subdirectory(external/glad)
target_link_libraries(runic PRIVATE glad)

# glfw
add_subdirectory(external/glfw)
target_link_libraries(runic PRIVATE glfw)

# glm
set(GLM_BUILD_TESTS OFF)
set(BUILD_SHARED_LIBS_SAVED "${BUILD_SHARED_LIBS}")
set(BUILD_SHARED_LIBS OFF)
add_subdirectory(external/glm)
set(BUILD_SHARED_LIBS "${BUILD_SHARED_LIBS_SAVED}")
target_link_libraries(runic PRIVATE glm::glm)
