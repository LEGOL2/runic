add_library(runic)
set(SRC
    runic.cppm
)
target_sources(runic PUBLIC FILE_SET CXX_MODULES FILES ${SRC})

# glad
add_subdirectory(external/glad)
target_link_libraries(runic PRIVATE glad)
